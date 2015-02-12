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

#ifndef html_yaccer_included
#define html_yaccer_included yes

#include "whatr_html_lexer.h"

class HTMLElement
{
public:
	// Html yaccer stuff:
	int type; // 0 = text, 1 = opening
	std::string text; // if (type==0) { this is text } else { this is tag name }
	std::vector<std::string> argNames;
	std::vector<std::string> argValues;
	HTMLElement* parent;
	std::vector<HTMLElement*> children;
	
	// TODO add an int containing the index of this element in its parent's children
	
	// Css yaccer stuff:
	std::vector<std::string> styleFields;
	std::vector<std::string> styleValues;
	
	// Prerenderer stuff:
	int sizeRelativity = 0; // 0 = no relativity, 1 = 
	int width = 0;
	int height = 0;
};
class htmlYaccArgs
{
public:
	int* lexingPage;
	int* yaccingPage;
	std::vector<HTMLTag>* HTMLTags;
	std::vector<HTMLElement*>* HTMLElements;
	htmlYaccArgs(int* lexingPage,
			int* yaccingPage,
			std::vector<HTMLTag>* HTMLTags,
			std::vector<HTMLElement*>* HTMLElements):
		lexingPage(lexingPage),
		yaccingPage(yaccingPage),
		HTMLTags(HTMLTags),
		HTMLElements(HTMLElements){};
};
void* htmlYaccThreadFunc(void* args);

#endif
