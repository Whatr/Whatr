/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#ifndef css_yaccer_included
#define css_yaccer_included yes

#include "css_lexer.h"
#include "css_selector.h"
#include "css_values.h"
#include "css_properties.h"

class CSSClass
{
public:
	CSSSelector selector;
	std::vector<CSSProperty> ruleProperties;
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
