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
#include "css_properties.h"

enum HTMLTagType
{
	TAG_DOCTYPE,
	TAG_A,
	TAG_ABBR,
	TAG_ACRONYM,
	TAG_ADDRESS,
	TAG_APPLET,
	TAG_AREA,
	TAG_ARTICLE,
	TAG_ASIDE,
	TAG_AUDIO,
	TAG_B,
	TAG_BASE,
	TAG_BASEFONT,
	TAG_BDI,
	TAG_BDO,
	TAG_BIG,
	TAG_BLOCKQUOTE,
	TAG_BODY,
	TAG_BR,
	TAG_BUTTON,
	TAG_CANVAS,
	TAG_CAPTION,
	TAG_CENTER,
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
	TAG_DIR,
	TAG_DIV,
	TAG_DL,
	TAG_DT,
	TAG_EM,
	TAG_EMBED,
	TAG_FIELDSET,
	TAG_FIGCAPTION,
	TAG_FIGURE,
	TAG_FONT,
	TAG_FOOTER,
	TAG_FORM,
	TAG_FRAME,
	TAG_FRAMESET,
	TAG_HEAD,
	TAG_HEADER,
	TAG_H1,
	TAG_H2,
	TAG_H3,
	TAG_H4,
	TAG_H5,
	TAG_H6,
	TAG_HR,
	TAG_HTML,
	TAG_I,
	TAG_IFRAME,
	TAG_IMG,
	TAG_INPUT,
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
	TAG_META,
	TAG_METER,
	TAG_NAV,
	TAG_NOFRAMES,
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
	TAG_STRIKE,
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
	TAG_TT,
	TAG_U,
	TAG_UL,
	TAG_VAR,
	TAG_VIDEO,
	TAG_WBR,
};

class HTMLElement
{
public:
	// Html yaccer stuff:
	int type; // 0 = text, 1 = opening
	ConstStr text; // if (type==0) { this is text } else { this is tag name }
	HTMLTagType tag;
	
	std::vector<ConstStr> argNames;
	std::vector<ConstStr> argValues;
	HTMLElement* parent;
	std::vector<HTMLElement*> children;
	
	// TODO add an int containing the index of this element in its parent's children
	
	// CSS applyer stuff:
	bool cssApplied;
	
	std::vector<CSSProperty> styleFields;
	std::vector<CSSValue> styleValues;
	
	CSSValue cssTop, cssLeft, cssBottom, cssRight;
	int cssColor;
	int cssBackgroundColor;
	CSSConstant cssBackgroundAttachment, cssBackgroundRepeat;
	CSSValue cssBackgroundPositionX, cssBackgroundPositionY;
	ConstStr cssBackgroundImage;
	int cssBorderBottomColor, cssBorderTopColor, cssBorderLeftColor, cssBorderRightColor;
	CSSConstant cssBorderBottomStyle, cssBorderTopStyle, cssBorderLeftStyle, cssBorderRightStyle;
	CSSValue cssBorderBottomWidth, cssBorderTopWidth, cssBorderLeftWidth, cssBorderRightWidth;
	CSSValue cssClear;
	CSSConstant cssDisplay;
	CSSConstant cssFloat;
	CSSValue cssWidth, cssHeight;
	CSSValue cssPaddingBottom, cssPaddingTop, cssPaddingLeft, cssPaddingRight;
	CSSConstant cssVerticalAlign;
	CSSValue cssMarginBottom, cssMarginTop, cssMarginLeft, cssMarginRight;
	CSSValue cssLetterSpacing;
	CSSValue cssLineHeight;
	CSSConstant cssPosition;
	CSSConstant cssTextAlign;
	CSSConstant cssTextTransform;
	CSSValue cssTextIndentLength;
	CSSConstant cssTextIndentType;
	CSSConstant cssWhiteSpace;
	CSSValue cssWordSpacing;
	CSSConstant cssTextDecorationLine, cssTextDecorationStyle, cssTextDecorationColor;
	
	
	void applyCSSDefaults()
	{
		//cssTop = 
	}
	
	
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
	HTMLElement* document;
	htmlYaccArgs(int* lexingPage,
			int* yaccingPage,
			std::vector<HTMLTag>* HTMLTags,
			HTMLElement* document):
		lexingPage(lexingPage),
		yaccingPage(yaccingPage),
		HTMLTags(HTMLTags),
		document(document){};
};
void* htmlYaccThreadFunc(void* args);

#endif
