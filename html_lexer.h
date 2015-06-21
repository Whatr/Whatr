/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#ifndef html_lexer_included
#define html_lexer_included yes

#include "const_str.h"
#include "html_tag_types.h"

class HTMLTag
{
public:
	int type; // 0 = text, 1 = opening, 2 = opening & self-closing, 3 = closing
	ConstStr text; // if (type==0) { this is text } else { this is tag name }
	HTMLTagType tag;
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
