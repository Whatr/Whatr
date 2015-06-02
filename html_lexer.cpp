/* Copyright (C) 2015 Jesse Busman
/* This program is free software: you can redistribute it and/or modify
/* it under the terms of the GNU General Public License as published by
/* the Free Software Foundation, either version 3 of the License, or
/* (at your option) any later version.
/* This program is distributed in the hope that it will be useful,
/* but WITHOUT ANY WARRANTY; without even the implied warranty of
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/* GNU General Public License for more details.
/* You should have received a copy of the GNU General Public License
/* along with this program.  If not, see <http://www.gnu.org/licenses/>  */

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <cctype>

#include "log_funcs.hpp"
#include "html_lexer.h"

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}
int isAcceptableHtmlTagNameFirstCharacter(char c)
{
	return	(c>='a' && c<='z') ||
			(c>='A' && c<='Z') ||
			c==':' ||
			c=='_';
}
int isAcceptableHtmlTagNameNonFirstCharacter(char c)
{
	return	isAcceptableHtmlTagNameFirstCharacter(c) ||
			(c>='0' && c<='9') ||
			c=='-' ||
			c=='.';
}
void* htmlLexThreadFunc(void* args)
{
	PRINT(htmlLexThreadFunc start);
	htmlLexArgs* l = (htmlLexArgs*)args;
	
	int* downloadingPage = l->downloadingPage;
	int* lexingPage = l->lexingPage;
	std::vector<HTMLTag>* HTMLTags = l->HTMLTags;
	std::vector<ConstStr>* headerFields = l->headerFields;
	std::vector<ConstStr>* headerValues = l->headerValues;
	ConstStr* downloadedHeaders = l->downloadedHeaders;
	ConstStr* downloadedHTML = l->downloadedHTML;
	
	*lexingPage = 1;
	PRINT(htmlLexThreadFunc has set lexingPage=1);
	////////////////////
	// Parse headers
	{
		ConstStrIterator i = downloadedHeaders->iterate();
		
		int inFirstLine = 1;
		int inField = 0;
		int inValue = 0;
		
		ConstStr httpVersion;
		int responseCode = -1;
		ConstStr responseStatus;
		
		int fieldStart = -1;
		int valueStart = -1;
		
		int newLinesInARow = 0;
		
		while
		(
			*downloadingPage
			||
			i<downloadedHeaders->length
		)
		{
			while(i>=downloadedHeaders->length)
			{
				if ((*downloadingPage)==0) break; // If downloading is done, quit
				usleep(100);
			} // Wait until more has been buffered
			if
			(
				(*downloadingPage)==0
				&&
				(i>=downloadedHeaders->length)
			)
			{
				PRINT(HTML Lex thread detected downloader=done);
				break; // If downloading is done, quit
			}
			char c = *i;
			if (inFirstLine)
			{
				if (inFirstLine==1) // In HTTP version
				{
					if (c==' ')
					{
						httpVersion = downloadedHeaders->subString(0, i.pos);
						printf("httpVersion=");httpVersion.printLine();
						inFirstLine++;
					}
				}
				else if (inFirstLine==2) // In response code
				{
					if (c==' ')
					{
						responseCode = atoi(downloadedHeaders->subString(httpVersion.length+1, i.pos-httpVersion.length-1).copy());
						printf("responseCode=%i\n", responseCode);
						inFirstLine++;
					}
				}
				else if (inFirstLine==3) // In response status
				{
					if (c=='\r' || c=='\n')
					{
						if (responseStatus.length==0)
						{
							responseStatus = downloadedHeaders->subString(httpVersion.length+1+3+1, i.pos-httpVersion.length-1-3-1);
							printf("responseStatus=");responseStatus.printLine();
							inFirstLine = 0;
							inField = 1;
							fieldStart = i.pos+1;
						}
					}
				}
			}
			else if (inField)
			{
				if (c==':')
				{
					newLinesInARow = 0;
					inField = 0;
					inValue = 1;
					valueStart = i.pos+2;
					
					headerFields->push_back(downloadedHeaders->subString(fieldStart, i.pos-fieldStart));
					i++;
				}
				else if (c=='\r' || c=='\n')
				{
					newLinesInARow++;
					if (newLinesInARow>=4)
					{
						break;
					}
					fieldStart = i.pos+1;
				}
				else
				{
					newLinesInARow = 0;
				}
			}
			else if (inValue)
			{
				if (c=='\r' || c=='\n')
				{
					newLinesInARow++;
					inValue = 0;
					inField = 1;
					fieldStart = i.pos+1;
					
					headerValues->push_back(downloadedHeaders->subString(valueStart, i.pos-valueStart));
				}
				else
				{
					newLinesInARow = 0;
				}
			}
			i++;
		}
		
	}
	
	printf("Starting Parse HTML...\n");
	
	////////////////////
	// Parse HTML
	{
		ConstStrIterator i = downloadedHTML->iterate();
		
		
		int tagType = 0; // 0 = text, 1 = opening, 2 = opening & self-closing, 3 = closing
		int inTag = 0; // 0 = inside text, 1 = inside tag name, 2 = inside argument field, 3 = inside argument value
		
		int inArgValueQuotes = 0; // 0 = no, 1 = yes
		char inArgValueQuotesType = 0; // ' or "
		int inArgValueBackslashed = 0; // 0 = no, 1 = yes
		
		HTMLTag tag;
		
		//std::string buffer;
		int bufferStart = -1;
		int tagTextStart = -1;
		
		int escaped = 0;
		
		int inComment = 0; // 1 = <, 2 = <!, 3 = <!-, 4 = <!--, 5 = -, 6 = --, 7 = -->
		int insideScript = 0; // 0 = no, 1 = yes
		std::string slashScript = std::string("");
		int slashScriptStart = 0;
		
		while
		(
			*downloadingPage
			||
			i<downloadedHTML->length
		)
		{
			//printf("2\n");
			char c = *i;
			
			if (!insideScript && c=='\n' || c=='\t') c = ' ';
			
			if (c=='<' && inComment==0) inComment++;
			else if (c=='!' && inComment==1) inComment++;
			else if (c=='-' && inComment==2) inComment++;
			else if (c=='-' && inComment==3)
			{
				HTMLTags->pop_back();
				inComment++;
			}
			
			else if (c=='-' && inComment==4) inComment++;
			else if (c=='-' && inComment==5) inComment++;
			else if (c=='>' && inComment==6)
			{
				//HTMLTags->pop_back();
				//tag.text = std::string("");
				inComment = 0;
				tag.text = tag.text.subString(1, tag.text.length-1);
				i++;
				continue;
			}
			else if (inComment>0 && inComment<4) inComment = 0;
			else if (inComment>4 && inComment<7) inComment = 4;
			
			
			printf("c=%c\n", c);
			
			
			if (inComment>=4)
			{
				i++;
				continue;
			}
			
			
			if (tagType==0 && inTag==0) // Not inside a tag
			{
				printf("5\n");
				if (c=='<' && !insideScript) {	// bla bla <div>
					tagType = 1;				//         ^
					inTag = 1;
					if (tagTextStart!=-1)
					{
						tag.type = 0;
						tag.text = downloadedHTML->subString(tagTextStart, i.pos-tagTextStart);
						HTMLTags->push_back(tag);
						tag = HTMLTag();
					}
					tagTextStart = i.pos+1;
				} else {			// bla bla
									//      ^
					//tag.text += c;
					if (tagTextStart==-1) tagTextStart = i.pos;
					if (insideScript) {
						if (
							(c==' ' || c=='\t')
							&&
							(
								slashScript.size()==1 ||	// < /script>
								slashScript.size()==2 ||	// </ script>
								slashScript.size()==8		// </script >
							)
							) {
						} else {
							printf("slashScript=%s\n", slashScript.c_str());
							if (slashScript.size()==0) {
								if (c=='<') {
									slashScript += '<';
									slashScriptStart = i.pos;
								}
								else slashScript = std::string("");
							}
							else if (slashScript.size()==1) {
								if (c=='/') slashScript += '/';
								else slashScript = std::string("");
							}
							else if (slashScript.size()==2) {
								if (c=='s') slashScript += 's';
								else slashScript = std::string("");
							}
							else if (slashScript.size()==3) {
								if (c=='c') slashScript += 'c';
								else slashScript = std::string("");
							}
							else if (slashScript.size()==4) {
								if (c=='r') slashScript += 'r';
								else slashScript = std::string("");
							}
							else if (slashScript.size()==5) {
								if (c=='i') slashScript += 'i';
								else slashScript = std::string("");
							}
							else if (slashScript.size()==6) {
								if (c=='p') slashScript += 'p';
								else slashScript = std::string("");
							}
							else if (slashScript.size()==7) {
								if (c=='t') slashScript += 't';
								else slashScript = std::string("");
							}
							else if (slashScript.size()==8) {
								if (c=='>') { // End of script!
									slashScript = std::string("");
									insideScript = 0;
									tag.type = 0;
									tag.text = downloadedHTML->subString(tagTextStart, slashScriptStart-tagTextStart);
									tagTextStart = -1;
									HTMLTags->push_back(tag);
									
									tag = HTMLTag();
									tag.type = 3;
									tag.text = std::string("script");
									HTMLTags->push_back(tag);
									tag = HTMLTag();
									inTag = 0;
									tagType = 0;
								}
								else slashScript = std::string("");
							}
						}
					}
				}
			}
			else if (tagType && inTag && c=='<' && !inArgValueQuotes)
			{	// Unexpected <
				printf("8\n");
				if (inTag==3)
				{
					tag.argValues.push_back(downloadedHTML->subString(bufferStart, i.pos-bufferStart));
				}
				std::cout << "Checkpoint\n";
				tag.type = tagType;
				int allArgValuesAreEmpty = 1;
				for (int j;j<tag.argValues.size();j++) {
					if (tag.argValues.at(j).length>0) {
						allArgValuesAreEmpty = 0;
						break;
					}
				}
				std::string untrimmed(tag.text.copy());
				std::string trimmed = trim(untrimmed);
				std::size_t found1 = trimmed.find(std::string(" "));
				std::size_t found2 = trimmed.find(std::string(","));
				std::size_t found4 = trimmed.find(std::string(";"));
				std::size_t found5 = trimmed.find(std::string("\n"));
				std::size_t found6 = trimmed.find(std::string("+"));
				std::size_t found7 = trimmed.find(std::string("("));
				std::size_t found8 = trimmed.find(std::string(")"));
				std::size_t found9 = trimmed.find(std::string("["));
				std::size_t found10 = trimmed.find(std::string("]"));
				std::size_t found11 = trimmed.find(std::string("{"));
				std::size_t found12 = trimmed.find(std::string("}"));
				std::size_t found13 = trimmed.find(std::string("/"));
				/*if (
					tag.text==std::string("b") &&
					tag.text==std::string("i") &&
					tag.text==std::string("u") &&
					allArgValuesAreEmpty) {
					// <b lorum ipsum</b>
				} else*/
				if (tag.text.length==0) {
					std::cout << RED << "Lexer error: Unexpected < - Assumed you meant &lt;\n" << NOCLR;
					int j = i.pos-1;
					for (;;j--)
					{
						if ((*downloadedHTML)[j] == '<')
						{
							break;
						}
					}
					tag.text = downloadedHTML->subString(j, i.pos-j);
					tag.type = 0;
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					inTag = 1;
					tagType = 1;
				} else if (
					found1==std::string::npos &&
					found2==std::string::npos &&
					found4==std::string::npos &&
					found5==std::string::npos &&
					found6==std::string::npos &&
					found7==std::string::npos &&
					found8==std::string::npos &&
					found9==std::string::npos &&
					found10==std::string::npos &&
					found11==std::string::npos &&
					found12==std::string::npos &&
					found13==std::string::npos &&
					(
						tag.argNames.size()==0 ||
						!allArgValuesAreEmpty
					)
				)
				{
					std::cout << RED << "Lexer error: Unexpected < - Assumed you forgot the > for a <" << trimmed << "> tag\n" << NOCLR;
					//std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(), ::tolower);
					tag.text = trimmed;
					HTMLTags->push_back(tag);
				
					tag = HTMLTag();
					tag.type = 1;
					inTag = 1;
					tagType = 1;
				} else {
					std::cout << RED << "Lexer error: Unexpected < - Assumed you meant &lt;\n" << NOCLR;
					int j = i.pos-1;
					for (;;j--)
					{
						if ((*downloadedHTML)[j] == '<')
						{
							break;
						}
					}
					tag.text = downloadedHTML->subString(j, i.pos-j);
					tag.type = 0;
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					inTag = 1;
					tagType = 1;
				}
			}
			else if (tagType==1 && inTag==1) // Inside an opening tag name
			{
				printf("9\n");
				if (c==' ' && tagTextStart==i.pos) { // < div id="bla">
					printf("10\n");
					// Ignore the space
					tagTextStart = i.pos+1;
				}
				else if (	tagTextStart==i.pos &&
							c!='/' &&
							!isAcceptableHtmlTagNameFirstCharacter(c)) {
					printf("11\n");
					if (c!='!') std::cout << RED << "Character " << c << " may not be the first character of a tag name.\n" << NOCLR;
					tag.text = std::string("<")+c;
					tag.type = 0;
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					inTag = 0;
					tagType = 0;
				} else if (c==' ') {	// <div id="bla">
					inTag = 2;			//     ^
					
					printf("12\n");
					tag.text = downloadedHTML->subString(tagTextStart, i.pos-tagTextStart);
					tagTextStart = -1;
					std::cout << "Tag possibly with arguments found: " << tag.text.copy() << "\n";
					bufferStart = i.pos+1;
				} else if (c=='/') {
					printf("13\n");
					if (tagTextStart==i.pos) // because the previous cycle made it i+1
					{					// </body>
						tagType = 3;	//  ^
						std::cout << "Closing tag start detected: </\n";
						tagTextStart++;
					}
					else
					{					// <br/>
						tagType = 2;	//    ^
						std::cout << "Forward slash after tag name\n";
					}
				} else if (c=='>') {	// <div>
					tagType = 0;		//     ^
					inTag = 0;
					
					printf("14\n");
					tag.type = 1;
					//std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);
					tag.text = downloadedHTML->subString(tagTextStart, i.pos-tagTextStart);
					tagTextStart = -1;
					printf("14.1\n");
					if (tag.text==std::string("script"))
					{
						printf("15\n");
						insideScript = 1;
						slashScript = std::string("");
					}
					else if (tag.text==std::string("")) {
						printf("16\n");
						tag.text = std::string("empty");
					}
					HTMLTags->push_back(tag);
					//std::cout << "Tag without arguments found: " << tag.text << "\n";
					tag = HTMLTag();
				} else {			// <div>
					//tag.text+=c;	//   ^
					printf("17\n");
				}
			}
			else if (tagType==1 && inTag==2) // Inside an opening tag argument field
			{
				if (c==' ') {
					printf("18\n");
					if (bufferStart<i.pos) {
						printf("18.1\n");
						//std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
						tag.argNames.push_back(downloadedHTML->subString(bufferStart, i.pos-bufferStart));
						std::cout << "Argument name found: " << tag.argNames.back().copy() << "\n";
					}
					bufferStart = i.pos+1;
				} else if (c=='=')
				{				// <div id="bla">
					inTag = 3;	//        ^
					if (tag.argNames.size()>tag.argValues.size()) {
						printf("19\n");
						bufferStart = i.pos+1;
					} else {
						//std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
						printf("20\n");
						tag.argNames.push_back(downloadedHTML->subString(bufferStart, i.pos-bufferStart));
						std::cout << "Argument name found: " << tag.argNames.back().copy() << "\n";
						bufferStart = i.pos+1;
					}
				} else if (c=='/') {
					printf("21\n");
					if (bufferStart<i.pos)
					{
						printf("21.1\n");
						//std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
						tag.argNames.push_back(downloadedHTML->subString(bufferStart, i.pos-bufferStart));
						tag.argValues.push_back(downloadedHTML->subString(0, 0));
					}
					tag.type = 2;
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					tagType = 0;
					inTag = 0;
				} else if (c=='>') {
					printf("22\n");
					while (tag.argValues.size()<tag.argNames.size())
					{
						tag.argValues.push_back(downloadedHTML->subString(0, 0));
					}
					if (bufferStart<i.pos)
					{
						//std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
						tag.argNames.push_back(downloadedHTML->subString(bufferStart, i.pos-bufferStart));
						tag.argValues.push_back(downloadedHTML->subString(0, 0));
					}
					if (tag.text==std::string("script"))
					{
						insideScript = 1;
						slashScript = std::string("");
					}
					tag.type = 1;
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					tagType = 0;
					inTag = 0;
				} else if (bufferStart>=i.pos &&
					!isAcceptableHtmlTagNameFirstCharacter(c)) {
					std::cout << RED << "Lexer error: " << c << " is not a valid attribute name first character!" << NOCLR;
				} else if (!isAcceptableHtmlTagNameNonFirstCharacter(c)) {
					std::cout << RED << "Lexer error: " << c << " is not a valid attribute name character!" << NOCLR;
				} else {		// <div id="bla">
					//buffer+=c;//      ^
					
					while (tag.argNames.size()>tag.argValues.size())
					{
						tag.argValues.push_back(downloadedHTML->subString(0, 0));
					}
					printf("23\n");
				}
			}
			else if (tagType==1 && inTag==3) // Inside an opening tag argument value
			{
				if (c==' ' && !inArgValueQuotes) {
					if (bufferStart>=i.pos) {
						printf("24\n");
						bufferStart = i.pos+1;
						// la;sdfkla;lskdfl;kasfdsa ignore the space
					} else {
						ConstStr argValue = downloadedHTML->subString(bufferStart, i.pos-bufferStart);
						if (argValue[0]=='"')
						{
							argValue = argValue.trim('"', '"', '"', '"');
						}
						else if (argValue[0]=='\'')
						{
							argValue = argValue.trim('\'', '\'', '\'', '\'');
						}
						tag.argValues.push_back(argValue);
						std::cout << "Argument value found: " << tag.argValues.back().copy() << "\n";
						bufferStart = i.pos+1;
						inTag = 2;
						printf("25\n");
					}
					inArgValueBackslashed = 0;
				} else if (c=='"' || c=='\'') {
					if (!inArgValueQuotes) {
						printf("26\n");
						if (bufferStart>=i.pos)	// <div id="bla">
						{								//         ^
							inArgValueQuotes = 1;
							inArgValueQuotesType = c;
						} else {
							inArgValueQuotes = 0;
						}
					} else if (inArgValueQuotes) {
						printf("27\n");
						if (inArgValueBackslashed)	// <b wc="Mr Fong said\"hi\"">
						{							//                         ^
							inArgValueBackslashed = 0;
							//if (inArgValueQuotesType!=c) buffer += '\\'; // TODO
							// buffer += c; // TODO
						} else if (c==inArgValueQuotesType) {
							inArgValueQuotes = 0;
						} //else buffer += c; // TODO
					}
				} else if (c=='/' && !inArgValueQuotes)
				{					// <br id="Mr Fong pressed enter"/>
					tag.type = 2;	//                               ^
					tagType = 2;
					inTag = 1;
					ConstStr argValue = downloadedHTML->subString(bufferStart, i.pos-bufferStart);
					if (argValue[0]=='"')
					{
						argValue = argValue.trim('"', '"', '"', '"');
					}
					else if (argValue[0]=='\'')
					{
						argValue = argValue.trim('\'', '\'', '\'', '\'');
					}
					tag.argValues.push_back(argValue);
					bufferStart = i.pos+1;
					printf("28\n");
					//std::cout << "Forward slash in tag outside quotes\n";
				} else if (c=='>') {
					if (inArgValueQuotes)	// <b wc="Mr Fong said <hi>">
					{						//                        ^
						printf("29\n");
						// buffer += '>'; // TODO
					} else {
						printf("30\n");
						if (tag.text==std::string("script"))
						{
							insideScript = 1;
							slashScript = std::string("");
						}
						std::cout << "Argument value found!\n";
						ConstStr argValue = downloadedHTML->subString(bufferStart, i.pos-bufferStart);
						if (argValue[0]=='"')
						{
							argValue = argValue.trim('"', '"', '"', '"');
						}
						else if (argValue[0]=='\'')
						{
							argValue = argValue.trim('\'', '\'', '\'', '\'');
						}
						argValue.printLine();
						tag.argValues.push_back(argValue);
						bufferStart = i.pos+1;
					
						//std::cout << "Tag <"<<tag.text<<"...> opened\n";
						
						tag.type = 1;
						//std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);

						HTMLTags->push_back(tag);
					
						tag = HTMLTag();
						tagType = 0;
						inTag = 0;
					}
					inArgValueBackslashed = 0;
				} else if (c=='\\' && inArgValueQuotes) {
					printf("31\n");
					if (inArgValueBackslashed) {
						inArgValueBackslashed = 0;
						//buffer += '\\'; // TODO
					} else {
						inArgValueBackslashed = 1;
					}
				} else {
					printf("32\n");
					if (!inArgValueQuotes) {
						if ((*downloadedHTML)[i.pos-1] == '"') {
							//buffer += '"'; // TODO
							inArgValueQuotes = 1;
						}
						else if ((*downloadedHTML)[i.pos-1] == '\'') {
							//buffer += '\''; // TODO
							inArgValueQuotes = 1;
						}
					}
					//buffer += c; // TODO
					inArgValueBackslashed = 0;
				}
			}
			else if (tagType==2 && inTag==2) {
				if (c==' '){}
				else if (c=='>') {
					printf("33\n");
					tag.text = downloadedHTML->subString(tagTextStart, i.pos-tagTextStart);
					tagTextStart = -1;
					std::cout << "Self-closing tag " << tag.text.copy() << " closed.\n";
					tag.type = 2;
					//std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);

					HTMLTags->push_back(tag);
					tag = HTMLTag();
					tagType = 0;
					inTag = 0;
				} else {
					printf("34\n");
					std::cout << RED << "Lexer error: Unexpected character " << c << " when a > was expected!\n" << NOCLR;
				}
			}
			else if (tagType==2 && inTag==1) {
				if (c==' '){}
				else if (c=='>') {
					printf("35\n");
					//std::cout << "Self-closing tag " << tag.text << " closed.\n";
					tag.type = 2;
					//std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					tagType = 0;
					inTag = 0;
				} else {
					std::cout << RED << "Lexer error: Unexpected character " << c << " when a > was expected!\n" << NOCLR;
				}
			} else if (tagType==3 && inTag==1) {
				if (c==' ' || c=='\n' || c=='\t') {
					printf("36\n");
					// Ignore the space
				} else if (c=='>') {
					printf("37\n");
					if (tagTextStart>=i.pos) {
						std::cout << RED << "Lexer error: Empty closing tag! Ignoring it.\n" << NOCLR;
						tag.type = 0;
						tagType = 0;
						inTag = 0;
						tag = HTMLTag();
					} else {
						tag.type = 3;
						//std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);
						tag.text = downloadedHTML->subString(tagTextStart, i.pos-tagTextStart);
						std::cout << "Closing tag closed: " << tag.text.copy() << "\n";
						tagTextStart = -1;
						HTMLTags->push_back(tag);
						tag = HTMLTag();
						tagType = 0;
						inTag = 0;
					}
				} else {
				}
			} else {
				std::cout << "FATAL LEXER ERROR: tagType=" << tagType
									<< " inTag=" << inTag << "\n";
			}
			i++;
		}
	}
	*lexingPage = 0;
	PRINT(htmlLexThreadFunc end);
	pthread_exit(NULL);
}
