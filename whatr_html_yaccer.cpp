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
#include <algorithm>
#include <vector>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <functional> 
#include <cctype>

#include "whatr_log_funcs.hpp"
#include "whatr_html_lexer.h"
#include "whatr_html_yaccer.h"

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
void* htmlYaccThreadFunc(void* args)
{
	PRINT(htmlYaccThreadFunc start);
	htmlYaccArgs* l = (htmlYaccArgs*)args;
	
	int* lexingPage = l->lexingPage;
	int* yaccingPage = l->yaccingPage;
	std::vector<HTMLTag>* HTMLTags = l->HTMLTags;
	std::vector<HTMLElement*>* HTMLElements = l->HTMLElements;
	*yaccingPage = 1;
	
	std::string misplacedText = std::string("");
	
	HTMLElement* currentElement = NULL;
	
	int i = 0;
	HTMLTag* currentTag = &((*HTMLTags)[0]);
	while
	(
		*lexingPage
		||
		i<HTMLTags->size()
	)
	{
		while
		(
			i>=HTMLTags->size()
			&&
			*lexingPage==1
		)
		{
			PRINT(HTML Yacc thread is waiting for lexer...);
			usleep(100000);
		}
		if
		(
			i>=HTMLTags->size()
			&&
			*lexingPage==0
		)
		{
			PRINT(HTML Yacc thread detected Lexer=done.);
			break;
		}
		
		//std::cout << "yacc:currentTag=" << currentTag->text << "\n";
		
		if (currentTag->type==1)
		{
			// The following tags are always self-closing:
			if (currentTag->text == std::string("meta") ||
				currentTag->text == std::string("br") ||
				currentTag->text == std::string("hr") ||
				currentTag->text == std::string("input") ||
				currentTag->text == std::string("img")
				)
			{
				currentTag->type = 2;
			}
			
			// The following tags are deprecated in HTML5
			if (currentTag->text == std::string("acronym") ||
				currentTag->text == std::string("applet") ||
				currentTag->text == std::string("basefont") ||
				currentTag->text == std::string("big") ||
				currentTag->text == std::string("center") ||
				currentTag->text == std::string("dir") ||
				currentTag->text == std::string("font") ||
				currentTag->text == std::string("frame") ||
				currentTag->text == std::string("frameset") ||
				currentTag->text == std::string("noframes") ||
				currentTag->text == std::string("strike") ||
				currentTag->text == std::string("tt"))
			{
				std::cout << RED << "Yacc warning: The <" << currentTag->text << "> tag is deprecated in HTML5.\n" << NOCLR;
			}
		}
		
		//---------Begin of the actual yaccing
		if (currentElement==NULL)
		{
			if (currentTag->type==0) // Text node
			{
				HTMLElement* el = new HTMLElement();
				el->type = 0;
				el->text = currentTag->text;
				el->parent = NULL;
				HTMLElements->push_back(el);
			}
			else if (currentTag->type==1) // Opening tag
			{
				HTMLElement* el = new HTMLElement();
				el->type = 1;
				el->text = currentTag->text;
				el->parent = NULL;
				el->argNames = currentTag->argNames;
				el->argValues = currentTag->argValues;
				HTMLElements->push_back(el);
				currentElement = HTMLElements->back();
			}
			else if (currentTag->type==2) // Opening & self-closing tag
			{
				HTMLElement* el = new HTMLElement();
				el->type = 1;
				el->text = currentTag->text;
				el->parent = NULL;
				el->argNames = currentTag->argNames;
				el->argValues = currentTag->argValues;
				HTMLElements->push_back(el);
			}
			else if (currentTag->type==3) // Closing tag
			{
				std::cout << RED << "Yacc error: Unexpected closing tag </" << currentTag->text << NOCLR << ">, there are no tags to close.\n";
			}
			else
			{
				std::cout << RED << "Yacc error: Unknown tag type " << currentTag->type << NOCLR << "\n";
			}
		}
		else
		{
			if (currentElement->text == std::string("body") &&
				currentElement->children.size() == 0)
			{
				if (misplacedText.length()!=0)
				{
					std::cout << RED << "Yacc warning: Added misplaced text on top of <body> contents\n";
					HTMLElement* el = new HTMLElement();
					el->type = 0;
					el->text = misplacedText;
					el->parent = currentElement;
					currentElement->children.push_back(el);
				}
			}
			if (currentTag->type==0) // Text node
			{
				if (currentElement->text == std::string("html") ||
					currentElement->text == std::string("head"))
				{
					// If text has accidentally been placed directly inside the
					// <head> or directly inside the <html>, we store it in
					// [std::string misplacedText] and later add it at the top of
					// the <body>
					std::string tttttt = trim(currentTag->text);
					if (tttttt.length()!=0)
					{
						std::cout << RED << "Yacc error: Misplaced text \""<<tttttt<<"\"\n";
						misplacedText += tttttt + "\n";
					}
				}
				else
				{
					HTMLElement* el = new HTMLElement();
					el->type = 0;
					el->text = currentTag->text;
					el->parent = currentElement;
					currentElement->children.push_back(el);
				}
			}
			else if (currentTag->type==1) // Opening tag
			{
				//std::cout << "Opening tag <" << currentTag->text << ">\n";
				HTMLElement* el = new HTMLElement();
				el->type = 1;
				el->text = currentTag->text;
				el->parent = currentElement;
				el->argNames = currentTag->argNames;
				el->argValues = currentTag->argValues;
				currentElement->children.push_back(el);
				currentElement = el;
			}
			else if (currentTag->type==2) // Opening & self-closing tag
			{
				//std::cout << "Opening & self-closing tag <" << currentTag->text << "/>\n";
				HTMLElement* el = new HTMLElement();
				el->type = 1;
				el->text = currentTag->text;
				el->parent = currentElement;
				el->argNames = currentTag->argNames;
				el->argValues = currentTag->argValues;
				currentElement->children.push_back(el);
			}
			else if (currentTag->type==3) // Closing tag
			{
				if (currentElement->text!=currentTag->text)
				{
					int found = 0;
					HTMLElement* checkingElement = currentElement;
					while (checkingElement!=NULL)
					{
						checkingElement = checkingElement->parent;
						if (checkingElement==NULL) break;
						if (checkingElement->text==currentTag->text)
						{
							found = 1;
							break;
						}
					}
					if (!found)
					{
						std::cout << RED << "Yacc error: Encountered unmatched closing tag </" << currentTag->text << "> inside <" << currentElement->text << "> tag. Ignoring it.\n" << NOCLR;
					}
					else
					{
						std::cout << RED << "Yacc error: Unexpected closing tag </" << currentTag->text << ">! The following closing tags were not there but have been assumed: ";
						checkingElement = currentElement;
						while (checkingElement!=NULL)
						{
							if (checkingElement->text==currentTag->text)
							{
								found = 1;
								break;
							}
							else
							{
								std::cout << "</" << checkingElement->text << "> ";
							}
							checkingElement = checkingElement->parent;
						}
						currentElement = checkingElement->parent;
						std::cout << NOCLR << "\n";
					}
				}
				else
				{
					currentElement = currentElement->parent;
				}
			}
			else
			{
				std::cout << RED << "Yacc error: Unknown tag type " << currentTag->type << NOCLR << "\n";
			}
		}
		
		//---------End of the actual yaccing
		i++;
		currentTag++;
	}
	while (currentElement!=NULL)
	{
		std::cout << RED << "Yacc error: Unclosed <" << currentElement->text << "> tag! \n" << NOCLR;
		currentElement = currentElement->parent;
	}
	*yaccingPage = 0;
	PRINT(htmlYaccThreadFunc end);
	pthread_exit(NULL);
}
