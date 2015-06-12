/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

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
HTMLTagType getTagType(ConstStr tag);
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
			
			
			////printf("c=%c\n", c);
			
			
			if (inComment>=4)
			{
				i++;
				continue;
			}
			
			
			if (tagType==0 && inTag==0) // Not inside a tag
			{
				//printf("5\n");
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
							//printf("slashScript=%s\n", slashScript.c_str());
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
									tag.tag = TAG_SCRIPT;
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
			{	// Unexpected <, ignore it
				HTMLTag tag;
				tag.type = 0;
				tag.text = downloadedHTML->subString(i.pos, 1);
				HTMLTags->push_back(tag);
				tagTextStart = i.pos+1;
				/*
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
				/*
				
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
				
				*//*
				char* blaStr = tag.text.copy();
				std::string untrimmed(blaStr);
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
				} else*//*
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
				}*/
			}
			else if (tagType==1 && inTag==1) // Inside an opening tag name
			{
				//printf("9\n");
				if (c==' ' && tagTextStart==i.pos) { // < div id="bla">
					//printf("10\n");
					// Ignore the space
					tagTextStart = i.pos+1;
				}
				else if (	tagTextStart==i.pos &&
							c!='/' &&
							!isAcceptableHtmlTagNameFirstCharacter(c)) {
					//printf("11\n");
					if (c!='!') std::cout << RED << "Character " << c << " may not be the first character of a tag name.\n" << NOCLR;
					tag.text = std::string("<")+c;
					tag.type = 0;
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					inTag = 0;
					tagType = 0;
				} else if (c==' ') {	// <div id="bla">
					inTag = 2;			//     ^
					
					//printf("12\n");
					tag.text = downloadedHTML->subString(tagTextStart, i.pos-tagTextStart);
					tag.tag = getTagType(tag.text);
					tagTextStart = -1;
					//std::cout << "Tag possibly with arguments found: " << tag.text.copy() << "\n";
					bufferStart = i.pos+1;
				} else if (c=='/') {
					//printf("13\n");
					if (tagTextStart==i.pos) // because the previous cycle made it i+1
					{					// </body>
						tagType = 3;	//  ^
						//std::cout << "Closing tag start detected: </\n";
						tagTextStart++;
					}
					else
					{					// <br/>
						tagType = 2;	//    ^
						//std::cout << "Forward slash after tag name\n";
					}
				} else if (c=='>') {	// <div>
					tagType = 0;		//     ^
					inTag = 0;
					
					//printf("14\n");
					tag.type = 1;
					//std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);
					tag.text = downloadedHTML->subString(tagTextStart, i.pos-tagTextStart);
					tagTextStart = -1;
					//printf("14.1\n");
					if (tag.text==std::string("script"))
					{
						//printf("15\n");
						insideScript = 1;
						slashScript = std::string("");
						tag.tag = TAG_SCRIPT;
					}
					else if (tag.text==std::string("")) {
						//printf("16\n");
						tag.text = std::string("empty");
						tag.tag = _TAG_CUSTOM;
					}
					else tag.tag = getTagType(tag.text);
					HTMLTags->push_back(tag);
					//std::cout << "Tag without arguments found: " << tag.text << "\n";
					tag = HTMLTag();
				} else {			// <div>
					//tag.text+=c;	//   ^
					//printf("17\n");
				}
			}
			else if (tagType==1 && inTag==2) // Inside an opening tag argument field
			{
				if (c==' ') {
					//printf("18\n");
					if (bufferStart<i.pos) {
						//printf("18.1\n");
						//std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
						tag.argNames.push_back(downloadedHTML->subString(bufferStart, i.pos-bufferStart));
						std::cout << "Argument name found: " << tag.argNames.back().copy() << "\n";
					}
					bufferStart = i.pos+1;
				} else if (c=='=')
				{				// <div id="bla">
					inTag = 3;	//        ^
					if (tag.argNames.size()>tag.argValues.size()) {
						//printf("19\n");
						bufferStart = i.pos+1;
					} else {
						//std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
						//printf("20\n");
						tag.argNames.push_back(downloadedHTML->subString(bufferStart, i.pos-bufferStart));
						std::cout << "Argument name found: " << tag.argNames.back().copy() << "\n";
						bufferStart = i.pos+1;
					}
				} else if (c=='/') {
					//printf("21\n");
					if (bufferStart<i.pos)
					{
						//printf("21.1\n");
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
					//printf("22\n");
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
					//printf("23\n");
				}
			}
			else if (tagType==1 && inTag==3) // Inside an opening tag argument value
			{
				if (c==' ' && !inArgValueQuotes) {
					if (bufferStart>=i.pos) {
						//printf("24\n");
						bufferStart = i.pos+1;
						// la;sdfkla;lskdfl;kasfdsa ignore the space
					} else {
						ConstStr argValue = downloadedHTML->subString(bufferStart, i.pos-bufferStart);
						if (argValue[0]=='"')
						{
							argValue = argValue.trim('"', '"', '"', '"', 1);
						}
						else if (argValue[0]=='\'')
						{
							argValue = argValue.trim('\'', '\'', '\'', '\'', 1);
						}
						tag.argValues.push_back(argValue);
						std::cout << "Argument value found: " << tag.argValues.back().copy() << "\n";
						bufferStart = i.pos+1;
						inTag = 2;
						//printf("25\n");
					}
					inArgValueBackslashed = 0;
				} else if (c=='"' || c=='\'') {
					if (!inArgValueQuotes) {
						if (bufferStart>=i.pos)	// <div id="bla">
						{						//         ^
							inArgValueQuotes = 1;
							inArgValueQuotesType = c;
							//printf("26.1\n");
						} else {
							inArgValueQuotes = 0;
							//printf("26.2\n");
						}
					} else if (inArgValueQuotes) {
						//printf("27\n");
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
						//printf("29\n");
						// buffer += '>'; // TODO
					} else {
						//printf("30\n");
						if (tag.text==std::string("script"))
						{
							insideScript = 1;
							slashScript = std::string("");
						}
						//std::cout << "Argument value found!\n";
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
					//printf("31\n");
					if (inArgValueBackslashed) {
						inArgValueBackslashed = 0;
						//buffer += '\\'; // TODO
					} else {
						inArgValueBackslashed = 1;
					}
				} else {
					//printf("32\n");
					if (!inArgValueQuotes && bufferStart+1>=i.pos) {
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
					//printf("33\n");
					tag.text = downloadedHTML->subString(tagTextStart, i.pos-tagTextStart);
					tag.tag = getTagType(tag.text);
					tagTextStart = -1;
					std::cout << "Self-closing tag " << tag.text.copy() << " closed.\n";
					tag.type = 2;
					//std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);

					HTMLTags->push_back(tag);
					tag = HTMLTag();
					tagType = 0;
					inTag = 0;
				} else {
					//printf("34\n");
					std::cout << RED << "Lexer error: Unexpected character " << c << " when a > was expected!\n" << NOCLR;
				}
			}
			else if (tagType==2 && inTag==1) {
				if (c==' '){}
				else if (c=='>') {
					//printf("35\n");
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
					//printf("36\n");
					// Ignore the space
				} else if (c=='>') {
					//printf("37\n");
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
						tag.tag = getTagType(tag.text);
						//std::cout << "Closing tag closed: " << tag.text.copy() << "\n";
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
HTMLTagType getTagType(ConstStr tag)
{
	if (tag-=std::string("!doctype")) return TAG_DOCTYPE;
	else if (tag-=std::string("a")) return TAG_A;
	else if (tag-=std::string("abbr")) return TAG_ABBR;
	else if (tag-=std::string("acronym")) return TAG_ACRONYM;
	else if (tag-=std::string("address")) return TAG_ADDRESS;
	else if (tag-=std::string("applet")) return TAG_APPLET;
	else if (tag-=std::string("area")) return TAG_AREA;
	else if (tag-=std::string("article")) return TAG_ARTICLE;
	else if (tag-=std::string("aside")) return TAG_ASIDE;
	else if (tag-=std::string("audio")) return TAG_AUDIO;
	else if (tag-=std::string("b")) return TAG_B;
	else if (tag-=std::string("base")) return TAG_BASE;
	else if (tag-=std::string("basefont")) return TAG_BASEFONT;
	else if (tag-=std::string("bdi")) return TAG_BDI;
	else if (tag-=std::string("bdo")) return TAG_BDO;
	else if (tag-=std::string("big")) return TAG_BIG;
	else if (tag-=std::string("blockquote")) return TAG_BLOCKQUOTE;
	else if (tag-=std::string("body")) return TAG_BODY;
	else if (tag-=std::string("br")) return TAG_BR;
	else if (tag-=std::string("button")) return TAG_BUTTON;
	else if (tag-=std::string("canvas")) return TAG_CANVAS;
	else if (tag-=std::string("caption")) return TAG_CAPTION;
	else if (tag-=std::string("center")) return TAG_CENTER;
	else if (tag-=std::string("cite")) return TAG_CITE;
	else if (tag-=std::string("code")) return TAG_CODE;
	else if (tag-=std::string("col")) return TAG_COL;
	else if (tag-=std::string("colgroup")) return TAG_COLGROUP;
	else if (tag-=std::string("datalist")) return TAG_DATALIST;
	else if (tag-=std::string("dd")) return TAG_DD;
	else if (tag-=std::string("del")) return TAG_DEL;
	else if (tag-=std::string("details")) return TAG_DETAILS;
	else if (tag-=std::string("dfn")) return TAG_DFN;
	else if (tag-=std::string("dialog")) return TAG_DIALOG;
	else if (tag-=std::string("dir")) return TAG_DIR;
	else if (tag-=std::string("div")) return TAG_DIV;
	else if (tag-=std::string("dl")) return TAG_DL;
	else if (tag-=std::string("dt")) return TAG_DT;
	else if (tag-=std::string("em")) return TAG_EM;
	else if (tag-=std::string("embed")) return TAG_EMBED;
	else if (tag-=std::string("fieldset")) return TAG_FIELDSET;
	else if (tag-=std::string("figcaption")) return TAG_FIGCAPTION;
	else if (tag-=std::string("figure")) return TAG_FIGURE;
	else if (tag-=std::string("font")) return TAG_FONT;
	else if (tag-=std::string("footer")) return TAG_FOOTER;
	else if (tag-=std::string("form")) return TAG_FORM;
	else if (tag-=std::string("frame")) return TAG_FRAME;
	else if (tag-=std::string("frameset")) return TAG_FRAMESET;
	else if (tag-=std::string("head")) return TAG_HEAD;
	else if (tag-=std::string("header")) return TAG_HEADER;
	else if (tag-=std::string("h1")) return TAG_H1;
	else if (tag-=std::string("h2")) return TAG_H2;
	else if (tag-=std::string("h3")) return TAG_H3;
	else if (tag-=std::string("h4")) return TAG_H4;
	else if (tag-=std::string("h5")) return TAG_H5;
	else if (tag-=std::string("h6")) return TAG_H6;
	else if (tag-=std::string("hr")) return TAG_HR;
	else if (tag-=std::string("html")) return TAG_HTML;
	else if (tag-=std::string("i")) return TAG_I;
	else if (tag-=std::string("iframe")) return TAG_IFRAME;
	else if (tag-=std::string("img")) return TAG_IMG;
	else if (tag-=std::string("input")) return TAG_INPUT;
	else if (tag-=std::string("ins")) return TAG_INS;
	else if (tag-=std::string("kbd")) return TAG_KBD;
	else if (tag-=std::string("keygen")) return TAG_KEYGEN;
	else if (tag-=std::string("label")) return TAG_LABEL;
	else if (tag-=std::string("legend")) return TAG_LEGEND;
	else if (tag-=std::string("li")) return TAG_LI;
	else if (tag-=std::string("link")) return TAG_LINK;
	else if (tag-=std::string("main")) return TAG_MAIN;
	else if (tag-=std::string("map")) return TAG_MAP;
	else if (tag-=std::string("mark")) return TAG_MARK;
	else if (tag-=std::string("menu")) return TAG_MENU;
	else if (tag-=std::string("menuitem")) return TAG_MENUITEM;
	else if (tag-=std::string("meta")) return TAG_META;
	else if (tag-=std::string("meter")) return TAG_METER;
	else if (tag-=std::string("nav")) return TAG_NAV;
	else if (tag-=std::string("noframes")) return TAG_NOFRAMES;
	else if (tag-=std::string("noscript")) return TAG_NOSCRIPT;
	else if (tag-=std::string("object")) return TAG_OBJECT;
	else if (tag-=std::string("ol")) return TAG_OL;
	else if (tag-=std::string("optgroup")) return TAG_OPTGROUP;
	else if (tag-=std::string("option")) return TAG_OPTION;
	else if (tag-=std::string("output")) return TAG_OUTPUT;
	else if (tag-=std::string("p")) return TAG_P;
	else if (tag-=std::string("param")) return TAG_PARAM;
	else if (tag-=std::string("pre")) return TAG_PRE;
	else if (tag-=std::string("progress")) return TAG_PROGRESS;
	else if (tag-=std::string("q")) return TAG_Q;
	else if (tag-=std::string("rp")) return TAG_RP;
	else if (tag-=std::string("rt")) return TAG_RT;
	else if (tag-=std::string("ruby")) return TAG_RUBY;
	else if (tag-=std::string("s")) return TAG_S;
	else if (tag-=std::string("samp")) return TAG_SAMP;
	else if (tag-=std::string("script")) return TAG_SCRIPT;
	else if (tag-=std::string("section")) return TAG_SECTION;
	else if (tag-=std::string("select")) return TAG_SELECT;
	else if (tag-=std::string("small")) return TAG_SMALL;
	else if (tag-=std::string("source")) return TAG_SOURCE;
	else if (tag-=std::string("span")) return TAG_SPAN;
	else if (tag-=std::string("strike")) return TAG_STRIKE;
	else if (tag-=std::string("strong")) return TAG_STRONG;
	else if (tag-=std::string("style")) return TAG_STYLE;
	else if (tag-=std::string("sub")) return TAG_SUB;
	else if (tag-=std::string("summary")) return TAG_SUMMARY;
	else if (tag-=std::string("sup")) return TAG_SUP;
	else if (tag-=std::string("table")) return TAG_TABLE;
	else if (tag-=std::string("tbody")) return TAG_TBODY;
	else if (tag-=std::string("td")) return TAG_TD;
	else if (tag-=std::string("textarea")) return TAG_TEXTAREA;
	else if (tag-=std::string("tfoot")) return TAG_TFOOT;
	else if (tag-=std::string("th")) return TAG_TH;
	else if (tag-=std::string("thead")) return TAG_THEAD;
	else if (tag-=std::string("time")) return TAG_TIME;
	else if (tag-=std::string("title")) return TAG_TITLE;
	else if (tag-=std::string("tr")) return TAG_TR;
	else if (tag-=std::string("track")) return TAG_TRACK;
	else if (tag-=std::string("tt")) return TAG_TT;
	else if (tag-=std::string("u")) return TAG_U;
	else if (tag-=std::string("ul")) return TAG_UL;
	else if (tag-=std::string("var")) return TAG_VAR;
	else if (tag-=std::string("video")) return TAG_VIDEO;
	else if (tag-=std::string("wbr")) return TAG_WBR;
	else return _TAG_CUSTOM;
}
