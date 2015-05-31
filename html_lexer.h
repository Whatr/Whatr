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

#ifndef html_lexer_included
#define html_lexer_included yes

#include "const_str.h"

class HTMLTag
{
public:
	int type; // 0 = text, 1 = opening, 2 = opening & self-closing, 3 = closing
	ConstStr text; // if (type==0) { this is text } else { this is tag name }
	std::vector<ConstStr> argNames;
	std::vector<ConstStr> argValues;
};
class htmlLexArgs
{
public:
	int* lexingPage;
	int* downloadingPage;
	std::vector<HTMLTag>* HTMLTags;
	std::vector<ConstStr>* headerFields;
	std::vector<ConstStr>* headerValues;
	ConstStr* downloadedHeaders;
	ConstStr* downloadedHTML;
	htmlLexArgs(int* downloadingPage,
			int* lexingPage,
			std::vector<HTMLTag>* HTMLTags,
			std::vector<ConstStr>* headerFields,
			std::vector<ConstStr>* headerValues,
			ConstStr* downloadedHeaders,
			ConstStr* downloadedHTML):
		downloadingPage(downloadingPage),
		lexingPage(lexingPage),
		HTMLTags(HTMLTags),
		headerFields(headerFields),
		headerValues(headerValues),
		downloadedHeaders(downloadedHeaders),
		downloadedHTML(downloadedHTML){};
};
void* htmlLexThreadFunc(void* args);

#endif
