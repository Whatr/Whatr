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

#ifndef css_lexer_included
#define css_lexer_included yes

class CSSToken
{
public:
	int type; // 0 = string, 1 = operator, 2 = percentage, 3 = number
	ConstStr text;
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
