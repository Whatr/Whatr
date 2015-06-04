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

#include "html_lexer.h"
#include "css_values.h"

class HTMLElement
{
public:
	// Html yaccer stuff:
	int type; // 0 = text, 1 = opening
	ConstStr text; // if (type==0) { this is text } else { this is tag name }
	std::vector<ConstStr> argNames;
	std::vector<ConstStr> argValues;
	HTMLElement* parent;
	std::vector<HTMLElement*> children;
	
	// TODO add an int containing the index of this element in its parent's children
	
	// CSS applyer stuff:
	std::vector<ConstStr> styleFields;
	std::vector<CSSValue> styleValues;
	int top = 0, left = 0, bottom = 0, right = 0;
	CSSDisplay cssDisplay;
	CSSPosition cssPosition;
	
	
	// Prerenderer stuff:
	int positionX = 0, positionY = 0;
	int width = 0;
	int height = 0;
	int paddingLeft = 0, paddingTop = 0, paddingRight = 0, paddingBottom = 0;
	int marginLeft = 0, marginTop = 0, marginRight = 0, marginBottom = 0;
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
