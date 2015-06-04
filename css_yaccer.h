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

#ifndef css_yaccer_included
#define css_yaccer_included yes

#include "css_lexer.h"
#include "css_selector.h"
#include "css_values.h"

class CSSClass
{
public:
	CSSSelector selector;
	std::vector<ConstStr> ruleNames;
	std::vector<CSSValue> ruleValues;
};
class cssYaccArgs
{
public:
	int* yaccingCSS;
	std::vector<CSSToken>* CSSTokens;
	std::vector<CSSClass>* CSSClasses;
	cssYaccArgs(int* yaccingCSS,
				std::vector<CSSToken>* CSSTokens,
				std::vector<CSSClass>* CSSClasses):
		yaccingCSS(yaccingCSS),
		CSSTokens(CSSTokens),
		CSSClasses(CSSClasses){};
};
void* cssYaccThreadFunc(void* args);

#endif
