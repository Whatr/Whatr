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

enum HTMLTagType
{
	TAG_DOCTYPE,
	TAG_A,
	TAG_ABBR,
	TAG_ADDRESS,
	TAG_AREA,
	TAG_ARTICLE,
	TAG_ASIDE,
	TAG_AUDIO,
	TAG_B,
	TAG_BASE,
	TAG_BDI,
	TAG_BDO,
	TAG_BLOCKQUOTE,
	TAG_BODY,
	TAG_BUTTON,
	TAG_CANVAS,
	TAG_CAPTION,
	TAG_CITE,
	TAG_CODE,
	TAG_COL,
	TAG_COLGROUP,
	TAG_DATALIST,
	TAG_DD,
	TAG_DEL,
	TAG_DETAILS,
	TAG_DFN,
	TAG_DIALOG,
	TAG_DIV,
	TAG_DL,
	TAG_DT,
	TAG_EM,
	TAG_EMBED,
	TAG_FIELDSET,
	TAG_FIGCAPTION,
	TAG_FIGURE,
	TAG_FOOTER,
	TAG_FORM,
	TAG_HEAD,
	TAG_HEADER,
	TAG_H1,
	TAG_H2,
	TAG_H3,
	TAG_H4,
	TAG_H5,
	TAG_H6,
	TAG_HTML,
	TAG_I,
	TAG_IFRAME,
	TAG_INS,
	TAG_KBD,
	TAG_KEYGEN,
	TAG_LABEL,
	TAG_LEGEND,
	TAG_LI,
	TAG_LINK,
	TAG_MAIN,
	TAG_MAP,
	TAG_MARK,
	TAG_MENU,
	TAG_MENUITEM,
	TAG_METER,
	TAG_NAV,
	TAG_NOSCRIPT,
	TAG_OBJECT,
	TAG_OL,
	TAG_OPTGROUP,
	TAG_OPTION,
	TAG_OUTPUT,
	TAG_P,
	TAG_PARAM,
	TAG_PRE,
	TAG_PROGRESS,
	TAG_Q,
	TAG_RP,
	TAG_RT,
	TAG_RUBY,
	TAG_S,
	TAG_SAMP,
	TAG_SCRIPT,
	TAG_SECTION,
	TAG_SELECT,
	TAG_SMALL,
	TAG_SOURCE,
	TAG_SPAN,
	TAG_STRONG,
	TAG_STYLE,
	TAG_SUB,
	TAG_SUMMARY,
	TAG_SUP,
	TAG_TABLE,
	TAG_TBODY,
	TAG_TD,
	TAG_TEXTAREA,
	TAG_TFOOT,
	TAG_TH,
	TAG_THEAD,
	TAG_TIME,
	TAG_TITLE,
	TAG_TR,
	TAG_TRACK,
	TAG_U,
	TAG_UL,
	TAG_VAR,
	TAG_VIDEO,
	TAG_WBR,
	
	_TAG_SELF_CLOSING_START,
	TAG_BR,
	TAG_HR,
	TAG_IMG,
	TAG_INPUT,
	TAG_META,
	_TAG_SELF_CLOSING_END,
	
	_TAG_DEPRECATED_START,
	TAG_ACRONYM,
	TAG_APPLET,
	TAG_BASEFONT,
	TAG_BIG,
	TAG_CENTER,
	TAG_DIR,
	TAG_FONT,
	TAG_FRAME,
	TAG_FRAMESET,
	TAG_NOFRAMES,
	TAG_STRIKE,
	TAG_TT,
	_TAG_DEPRECATED_END,
	
	_TAG_CUSTOM,
};

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
