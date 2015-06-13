/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#include <iostream>
#include <vector>
#include <string>

#include "log_funcs.hpp"
#include "html_yaccer.h"
#include "css_yaccer.h"
#include "css_applyer.h"
#include "css_selector.h"
#include "css_values.h"

void* cssApplyThreadFunc(void* args)
{
	PRINT(cssApplyThreadFunc start);
	
	cssApplyArgs* c = (cssApplyArgs*)args;
	
	int* applyingCSS = c->applyingCSS;
	std::vector<CSSClass>* CSSClasses = c->CSSClasses;
	HTMLElement* destination = c->destination;
	
	*applyingCSS = 1;
	PRINT(cssApplyThreadFunc has set applyingCss=1);
	PRINT(Applying CSS:);
	
	for (	std::vector<CSSClass>::iterator c=CSSClasses->begin();
			c!=CSSClasses->end();
			c++)
	{
		std::cout << "## ------ Entering a CSS class...\n";
		CSSSelector* selector = &(c->selector);
		std::vector<HTMLElement*> applyToList = CSSSelect(destination, selector);
		if (applyToList.size()==0);// std::cout << "CSSSelect returned empty vector\n";
		else for (	std::vector<HTMLElement*>::iterator el=applyToList.begin();
					el!=applyToList.end();
					el++)
		{
			(*el)->applyCSSDefaults();
			for (int i=0;i<c->ruleProperties.size();i++)
			{
				CSSProperty name = c->ruleProperties.at(i);
				CSSValue value = c->ruleValues.at(i);
				//std::cout << "CSSSelect returned: ";
				//std::cout << (*el)->text << "\n";
				(*el)->styleFields.push_back(name);
				(*el)->styleValues.push_back(value);
				switch (name)
				{
			case TOP: (*el)->cssTop = value; break;
			case LEFT: (*el)->cssLeft = value; break;
			case BOTTOM: (*el)->cssBottom = value; break;
			case RIGHT: (*el)->cssRight = value; break;
			case COLOR: (*el)->cssColor = value.colorValue; break;
			case BACKGROUND_COLOR: (*el)->cssBackgroundColor = value.colorValue; break;
			case BACKGROUND_ATTACHMENT: (*el)->cssBackgroundAttachment = value.constant; break;
			case BACKGROUND_REPEAT: (*el)->cssBackgroundRepeat = value.constant; break;
			case BACKGROUND_POSITION_X: (*el)->cssBackgroundPositionX = value; break;
			case BACKGROUND_POSITION_Y: (*el)->cssBackgroundPositionY = value; break;
			case BACKGROUND_IMAGE: (*el)->cssBackgroundImage = value.textValue; break;
			case BORDER_BOTTOM_COLOR: (*el)->cssBorderBottomColor = value.colorValue; break;
			case BORDER_BOTTOM_STYLE: (*el)->cssBorderBottomStyle = value.constant; break;
			case BORDER_BOTTOM_WIDTH: (*el)->cssBorderBottomWidth = value; break;
			case BORDER_LEFT_COLOR: (*el)->cssBorderLeftColor = value.colorValue; break;
			case BORDER_LEFT_STYLE: (*el)->cssBorderLeftStyle = value.constant; break;
			case BORDER_LEFT_WIDTH: (*el)->cssBorderLeftWidth = value; break;
			case BORDER_TOP_COLOR: (*el)->cssBorderTopColor = value.colorValue; break;
			case BORDER_TOP_STYLE: (*el)->cssBorderTopStyle = value.constant; break;
			case BORDER_TOP_WIDTH: (*el)->cssBorderTopWidth = value; break;
			case BORDER_RIGHT_COLOR: (*el)->cssBorderRightColor = value.colorValue; break;
			case BORDER_RIGHT_STYLE: (*el)->cssBorderRightStyle = value.constant; break;
			case BORDER_RIGHT_WIDTH: (*el)->cssBorderRightWidth = value; break;
			case CLEAR: (*el)->cssClear = value.constant; break;
			case DISPLAY: (*el)->cssDisplay = value.constant; break;
			case FLOAT: (*el)->cssFloat = value.constant; break;
			case FONT_VARIANT: (*el)->cssFontVariant = value.constant; break;
			case FONT_STYLE: (*el)->cssFontStyle = value.constant; break;
			case FONT_STRETCH: (*el)->cssFontStretch = value.constant; break;
			case FONT_WEIGHT: (*el)->cssFontWeight = value.constant; break;
			case FONT_SIZE: (*el)->cssFontSize = value; break;
			case FONT_FAMILY: (*el)->cssFontFamily = value.font; break;
			case WIDTH: (*el)->cssWidth = value; break;
			case HEIGHT: (*el)->cssHeight = value; break;
			case PADDING_BOTTOM: (*el)->cssPaddingBottom = value; break;
			case PADDING_TOP: (*el)->cssPaddingTop = value; break;
			case PADDING_LEFT: (*el)->cssPaddingLeft = value; break;
			case PADDING_RIGHT: (*el)->cssPaddingRight = value; break;
			case VERTICAL_ALIGN: (*el)->cssVerticalAlign = value.constant; break;
			case MARGIN_BOTTOM: (*el)->cssMarginBottom = value; break;
			case MARGIN_TOP: (*el)->cssMarginTop = value; break;
			case MARGIN_LEFT: (*el)->cssMarginLeft = value; break;
			case MARGIN_RIGHT: (*el)->cssMarginRight = value; break;
			case LETTER_SPACING: (*el)->cssLetterSpacing = value; break;
			case LINE_HEIGHT: (*el)->cssLineHeight = value; break;
			case LIST_STYLE_TYPE: (*el)->cssListStyleType = value.constant; break;
			case LIST_STYLE_POSITION: (*el)->cssListStylePosition = value.constant; break;
			case LIST_STYLE_IMAGE: (*el)->cssListStyleImage = value.textValue; break;
			case POSITION: (*el)->cssPosition = value.constant; break;
			case TEXT_ALIGN: (*el)->cssTextAlign = value.constant; break;
			case TEXT_TRANSFORM: (*el)->cssTextTransform = value.constant; break;
			case TEXT_INDENT_LENGTH: (*el)->cssTextIndentLength = value; break;
			case TEXT_INDENT_TYPE: (*el)->cssTextIndentType = value.constant; break;
			case WHITE_SPACE: (*el)->cssWhiteSpace = value.constant; break;
			case WORD_SPACING: (*el)->cssWordSpacing = value; break;
			case TEXT_DECORATION_LINE: (*el)->cssTextDecorationLine = value.constant; break;
			case TEXT_DECORATION_STYLE: (*el)->cssTextDecorationStyle = value.constant; break;
			case TEXT_DECORATION_COLOR: (*el)->cssTextDecorationColor = value.colorValue; break;
			case BORDER_SPACING: (*el)->cssBorderSpacing = value; break;
			case CLIP: (*el)->cssClip = value.constant; break;
			case Z_INDEX: (*el)->cssZIndex = value.lengthValue; break;
			case MAX_HEIGHT: (*el)->cssMaxHeight = value; break;
			case MAX_WIDTH: (*el)->cssMaxWidth = value; break;
			case MIN_HEIGHT: (*el)->cssMinHeight = value; break;
			case MIN_WIDTH: (*el)->cssMinWidth = value; break;
			case VISIBILITY: (*el)->cssVisibility = value.constant; break;
			case DIRECTION: (*el)->cssDirection = value.constant; break;
			case UNICODE_BIDI: (*el)->cssUnicodeBidi = value.constant; break; // wtf is that
			case BORDER_COLLAPSE: (*el)->cssBorderCollapse = value.constant; break;
			case CAPTION_SIDE: (*el)->cssCaptionSide = value.constant; break;
			case EMPTY_CELLS: (*el)->cssEmptyCells = value.constant; break;
			case TABLE_LAYOUT: (*el)->cssTableLayout = value.constant; break;
			case COUNTER_INCREMENT:(*el)->cssCounterIncrement=value.constant;break;
			case COUNTER_RESET: (*el)->cssCounterReset = value.constant; break;
			case CONTENT: (*el)->cssContent = value.textValue; break;
			case CURSOR: (*el)->cssCursor = value.constant; break;
			case OUTLINE_COLOR: (*el)->cssOutlineColor = value.colorValue; break;
			case OUTLINE_STYLE: (*el)->cssOutlineStyle = value.constant; break;
			case OUTLINE_WIDTH: (*el)->cssOutlineWidth = value; break;
			case PAGE_BREAK_AFTER: (*el)->cssPageBreakAfter = value.constant; break;
			case PAGE_BREAK_BEFORE: (*el)->cssPageBreakBefore=value.constant; break;
			case PAGE_BREAK_INSIDE: (*el)->cssPageBreakInside=value.constant; break;
			case QUOTES: (*el)->cssQuotes = value.constant; break;
			case WIDOWS: (*el)->cssWidows = value.constant; break;
			case ORPHANS: (*el)->cssOrphans = value.constant; break;
//			case : (*el)->css = ; break;
					default:
						if (name>=COMBOS)
						{
							std::cout << RED << "CSS error: a combo property was received by css_applyer.cpp!!!\n" << NOCLR;
						}
						else
						{
							std::cout << RED << "TODO: add css property to the list in css_applyer.cpp, name="<<name<<"\n" << NOCLR;
						}
					break;
				}
			}
		}
	}
	
	PRINT(Applying CSS done!);
	*applyingCSS = 0;
}
