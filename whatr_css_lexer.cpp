#include "whatr_css_lexer.h"

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <cctype>

#include "whatr_log_funcs.hpp"

void* cssLexThreadFunc(void* args)
{
	PRINT(cssLexThreadFunc start);
	
	cssLexArgs* c = (cssLexArgs*)args;
	
	std::vector<CSSToken>* CSSTokens = l->CSSTokens;
	
	*lexingCss = 1;
	PRINT(cssLexThreadFunc has set lexingCss=1);
	
	////////////////////
	// Lex CSS
	{
		int i = 0;
		
		HTMLToken token;
		
		while
		(
			*downloadingPage
			||
			i<downloadedHTML->length()
		)
		{
			char c = downloadedHTML->at(i);
			
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
				tag.text = tag.text.substr(1, tag.text.length()-1);
				i++;
				continue;
			}
			else if (inComment>0 && inComment<4) inComment = 0;
			else if (inComment>4 && inComment<7) inComment = 4;
			
			
			
			
			if (inComment>=4)
			{
				i++;
				continue;
			}
			
			//std::cout << "Char " << c << ": ";
			if (tagType==0 && inTag==0) // Not inside a tag
			{
				if (c=='<' && !insideScript) {	// bla bla <div>
					tagType = 1;				//         ^
					inTag = 1;
					if (tag.text!=std::string(""))
					{
						//std::cout << "Text node found: " << tag.text << "\n";
						tag.type = 0;
						HTMLTags->push_back(tag);
						tag = HTMLTag();
					}
				} else {			// bla bla
									//      ^
					tag.text += c;
					if (insideScript) {
						if (c!=' ' && c!='\t') {
							if (slashScript.size()==0) {
								if (c=='<') {
									slashScript += '<';
									slashScriptStart = i;
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
									tag.text = tag.text.substr(0, tag.text.length()-(i-slashScriptStart)-1);
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
				if (inTag==3)
				{
					tag.argValues.push_back(std::string(buffer));
				}
				std::cout << "Checkpoint\n";
				tag.type = tagType;
				int allArgValuesAreEmpty = 1;
				for (int j;j<tag.argValues.size();j++) {
					if (tag.argValues.at(j)!=std::string("")) {
						allArgValuesAreEmpty = 0;
						break;
					}
				}
				std::string trimmed = trim(tag.text);
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
				if (tag.text==std::string("")) {
					std::cout << RED << "Lexer error: Unexpected < - Assumed you meant &lt;\n" << NOCLR;
					int j = i-1;
					for (;;j--)
					{
						if (downloadedHTML->at(j) == '<')
						{
							break;
						}
					}
					tag.text = downloadedHTML->substr(j, i-j);
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
					std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(), ::tolower);
					tag.text = trimmed;
					HTMLTags->push_back(tag);
				
					tag = HTMLTag();
					tag.type = 1;
					inTag = 1;
					tagType = 1;
				} else {
					std::cout << RED << "Lexer error: Unexpected < - Assumed you meant &lt;\n" << NOCLR;
					int j = i-1;
					for (;;j--)
					{
						if (downloadedHTML->at(j) == '<')
						{
							break;
						}
					}
					tag.text = downloadedHTML->substr(j, i-j);
					tag.type = 0;
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					inTag = 1;
					tagType = 1;
				}
			}
			else if (tagType==1 && inTag==1) // Inside an opening tag name
			{
				if (c==' ' && tag.text==std::string("")) { // < div id="bla">
					// Ignore the space
				}
				else if (	tag.text==std::string("") &&
							c!='/' &&
							!isAcceptableHtmlTagNameFirstCharacter(c)) {
					if (c!='!') std::cout << RED << "Character " << c << " may not be the first character of a tag name.\n" << NOCLR;
					tag.text = std::string("<")+c;
					tag.type = 0;
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					inTag = 0;
					tagType = 0;
				} else if (c==' ') {	// <div id="bla">
					inTag = 2;			//     ^
					//std::cout << "Tag possibly with arguments found: " << tag.text << "\n";
					buffer = std::string("");
				} else if (c=='/') {
					if (tag.text==std::string(""))
					{					// </body>
						tagType = 3;	//  ^
						//std::cout << "Closing tag start detected: </\n";
					}
					else
					{					// <br/>
						tagType = 2;	//    ^
						//std::cout << "Forward slash after tag name\n";
					}
				} else if (c=='>') {	// <div>
					tagType = 0;		//     ^
					inTag = 0;
					
					tag.type = 1;
					std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);
					if (tag.text==std::string("script"))
					{
						insideScript = 1;
						slashScript = std::string("");
					}
					else if (tag.text==std::string("")) {
						tag.text = std::string("empty");
					}
					HTMLTags->push_back(tag);
					//std::cout << "Tag without arguments found: " << tag.text << "\n";
					tag = HTMLTag();
				} else {			// <div>
					tag.text += c;	//   ^
				}
			}
			else if (tagType==1 && inTag==2) // Inside an opening tag argument field
			{
				if (c==' ') {
					if (buffer!=std::string("")) {
						//std::cout << "Argument name without value found: " << buffer << "\n";
						std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
						tag.argNames.push_back(buffer);
						tag.argValues.push_back(std::string(""));
						buffer = std::string("");
					}
				} else if (c=='=')
				{				// <div id="bla">
					inTag = 3;	//        ^
					if (buffer==std::string("")) {
						tag.argValues.pop_back();
					} else {
						//std::cout << "Argument name found: " << buffer << "\n";
						std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
						tag.argNames.push_back(buffer);
						buffer = std::string("");
					}
				} else if (c=='/') {
					if (buffer!=std::string(""))
					{
						std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
						tag.argNames.push_back(buffer);
						tag.argValues.push_back(std::string(""));
					}
					tag.type = 2;
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					tagType = 0;
					inTag = 0;
				} else if (c=='>') {
					if (buffer!=std::string(""))
					{
						std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
						tag.argNames.push_back(buffer);
						tag.argValues.push_back(std::string(""));
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
				} else if (buffer==std::string("") &&
					!isAcceptableHtmlTagNameFirstCharacter(c)) {
					std::cout << RED << "Lexer error: " << c << " is not a valid attribute name first character!" << NOCLR;
				} else if (!isAcceptableHtmlTagNameNonFirstCharacter(c)) {
					std::cout << RED << "Lexer error: " << c << " is not a valid attribute name character!" << NOCLR;
				} else {		// <div id="bla">
					buffer += c;//      ^
				}
			}
			else if (tagType==1 && inTag==3) // Inside an opening tag argument value
			{
				if (c==' ' && !inArgValueQuotes) {
					if (buffer==std::string("")) {
						// la;sdfkla;lskdfl;kasfdsa ignore the space
					} else {
						//std::cout << "Argument value found: " << buffer << "\n";
						tag.argValues.push_back(buffer);
						buffer = std::string("");
						inTag = 2;
					}
					inArgValueBackslashed = 0;
				} else if (c=='"' || c=='\'') {
					if (!inArgValueQuotes) {
						if (buffer==std::string(""))	// <div id="bla">
						{								//         ^
							inArgValueQuotes = 1;
							inArgValueQuotesType = c;
						} else {
							inArgValueQuotes = 0;
						}
					} else if (inArgValueQuotes) {
						if (inArgValueBackslashed)	// <b wc="Mr Fong said\"hi\"">
						{							//                         ^
							inArgValueBackslashed = 0;
							if (inArgValueQuotesType!=c) buffer += '\\';
							buffer += c;
						} else if (c==inArgValueQuotesType) {
							inArgValueQuotes = 0;
						} else buffer += c;
					}
				} else if (c=='/' && !inArgValueQuotes)
				{					// <br id="Mr Fong pressed enter"/>
					tag.type = 2;	//                               ^
					tagType = 2;
					inTag = 1;
					tag.argValues.push_back(buffer);
					buffer = std::string("");
					//std::cout << "Forward slash in tag outside quotes\n";
				} else if (c=='>') {
					if (inArgValueQuotes)	// <b wc="Mr Fong said <hi>">
					{						//                        ^
						buffer += '>';
					} else {
						if (tag.text==std::string("script"))
						{
							insideScript = 1;
							slashScript = std::string("");
						}
						//std::cout << "Argument value found: " << buffer << "\n";
						tag.argValues.push_back(buffer);
						buffer = std::string("");
					
						//std::cout << "Tag <"<<tag.text<<"...> opened\n";
						
						tag.type = 1;
						std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);

						HTMLTags->push_back(tag);
					
						tag = HTMLTag();
						tagType = 0;
						inTag = 0;
					}
					inArgValueBackslashed = 0;
				} else if (c=='\\' && inArgValueQuotes) {
					if (inArgValueBackslashed) {
						inArgValueBackslashed = 0;
						buffer += '\\';
					} else {
						inArgValueBackslashed = 1;
					}
				} else {
					if (!inArgValueQuotes) {
						if (downloadedHTML->at(i-1) == '"') {
							buffer += '"';
							inArgValueQuotes = 1;
						}
						else if (downloadedHTML->at(i-1) == '\'') {
							buffer += '\'';
							inArgValueQuotes = 1;
						}
					}
					buffer += c;
					inArgValueBackslashed = 0;
				}
			}
			else if (tagType==2 && inTag==2) {
				if (c==' '){}
				else if (c=='>') {
					//std::cout << "Self-closing tag " << tag.text << " closed.\n";
					tag.type = 2;
					std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);

					HTMLTags->push_back(tag);
					tag = HTMLTag();
					tagType = 0;
					inTag = 0;
				} else {
					std::cout << RED << "Lexer error: Unexpected character " << c << " when a > was expected!\n" << NOCLR;
				}
			}
			else if (tagType==2 && inTag==1) {
				if (c==' '){}
				else if (c=='>') {
					//std::cout << "Self-closing tag " << tag.text << " closed.\n";
					tag.type = 2;
					std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);
					HTMLTags->push_back(tag);
					tag = HTMLTag();
					tagType = 0;
					inTag = 0;
				} else {
					std::cout << RED << "Lexer error: Unexpected character " << c << " when a > was expected!\n" << NOCLR;
				}
			} else if (tagType==3 && inTag==1) {
				if (c==' ' || c=='\n' || c=='\t') {
					// Ignore the space
				} else if (c=='>') {
					if (tag.text==std::string("")) {
						std::cout << RED << "Lexer error: Empty closing tag! Ignoring it.\n" << NOCLR;
						tag.type = 0;
						tagType = 0;
						inTag = 0;
						tag = HTMLTag();
					} else {
						//std::cout << "Closing tag closed: " << tag.text << "\n";
						tag.type = 3;
						std::transform(tag.text.begin(), tag.text.end(), tag.text.begin(), ::tolower);
						HTMLTags->push_back(tag);
						tag = HTMLTag();
						tagType = 0;
						inTag = 0;
					}
				} else {
					tag.text += c;
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
