/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#ifndef html_yaccer_included
#define html_yaccer_included yes

#include "html_lexer.h"
#include "css_values.h"
#include "css_properties.h"

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
	std::vector<CSSProperty> styleFields;
	std::vector<CSSValue> styleValues;
	
	bool cssApplied;
	
	CSSValue cssTop, cssLeft, cssBottom, cssRight;
	int cssColor;
	int cssBackgroundColor;
	CSSConstant cssBackgroundAttachment, cssBackgroundRepeat;
	CSSValue cssBackgroundPositionX, cssBackgroundPositionY;
	ConstStr cssBackgroundImage;
	int cssBorderBottomColor, cssBorderTopColor, cssBorderLeftColor, cssBorderRightColor;
	CSSConstant cssBorderBottomStyle, cssBorderTopStyle, cssBorderLeftStyle, cssBorderRightStyle;
	CSSValue cssBorderBottomWidth, cssBorderTopWidth, cssBorderLeftWidth, cssBorderRightWidth;
	CSSConstant cssClear;
	CSSConstant cssDisplay;
	CSSConstant cssFloat;
	CSSConstant cssFontVariant, cssFontStyle, cssFontStretch, cssFontWeight;
	CSSValue cssFontSize;
	Font* cssFontFamily;
	CSSValue cssWidth, cssHeight;
	CSSValue cssPaddingBottom, cssPaddingTop, cssPaddingLeft, cssPaddingRight;
	CSSConstant cssVerticalAlign;
	CSSValue cssMarginBottom, cssMarginTop, cssMarginLeft, cssMarginRight;
	CSSValue cssLetterSpacing;
	CSSValue cssLineHeight;
	CSSConstant cssListStyleType, cssListStylePosition;
	ConstStr cssListStyleImage;
	CSSConstant cssPosition;
	CSSConstant cssTextAlign;
	CSSConstant cssTextTransform;
	CSSValue cssTextIndentLength;
	CSSConstant cssTextIndentType;
	CSSConstant cssWhiteSpace;
	CSSValue cssWordSpacing;
	CSSConstant cssTextDecorationLine, cssTextDecorationStyle;
	int cssTextDecorationColor;
	CSSValue cssBorderSpacing;
	CSSConstant cssClip;
	int cssZIndex;
	CSSValue cssMaxHeight, cssMaxWidth, cssMinHeight, cssMinWidth;
	CSSConstant cssVisibility;
	CSSConstant cssDirection, cssUnicodeBidi;
	CSSConstant cssBorderCollapse;
	CSSConstant cssEmptyCells, cssTableLayout;
	CSSConstant cssCounterIncrement, cssCounterReset;
	ConstStr cssContent;
	CSSConstant cssCursor;
	int cssOutlineColor;
	CSSConstant cssOutlineStyle;
	CSSValue cssOutlineWidth;
	CSSConstant cssPageBreakAfter, cssPageBreakBefore, cssPageBreakInside;
	CSSConstant cssQuotes, cssWidows, cssOrphans;
	CSSConstant cssCaptionSide;
	
	void applyCSSDefaults();
	
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
	std::vector<HTMLElement*>* styles;
	htmlYaccArgs(int* lexingPage,
			int* yaccingPage,
			std::vector<HTMLTag>* HTMLTags,
			HTMLElement* document,
			std::vector<HTMLElement*>* styles):
		lexingPage(lexingPage),
		yaccingPage(yaccingPage),
		HTMLTags(HTMLTags),
		document(document),
		styles(styles){};
};
void* htmlYaccThreadFunc(void* args);

#endif
