/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#ifndef css_lexer_included
#define css_lexer_included yes

#include "const_str.h"

enum CSSTokenType
{
	TOKEN_TYPE_NOTHING,
	
	TOKEN_TYPE_STRING_NO_QUOTES,
	TOKEN_TYPE_OPERATOR,
	TOKEN_TYPE_PERCENTAGE,
	TOKEN_TYPE_NUMBER,
	TOKEN_TYPE_STRING_DOUBLE_QUOTES,
	TOKEN_TYPE_STRING_SINGLE_QUOTES,
};
class CSSToken
{
public:
	CSSTokenType type; // 0 = string, 1 = operator, 2 = percentage, 3 = number, 4 = string in double quotes, 5 = string in single quotes
	ConstStr text;
	CSSToken():
		text(ConstStr()){};
};
class cssLexArgs
{
public:
	int* lexingCSS;
	std::vector<CSSToken>* CSSTokens;
	ConstStr inputCSS;
	cssLexArgs(int* lexingCSS,
			std::vector<CSSToken>* CSSTokens,
			ConstStr inputCSS):
		lexingCSS(lexingCSS),
		CSSTokens(CSSTokens),
		inputCSS(inputCSS){};
};
void* cssLexThreadFunc(void* args);

#endif
