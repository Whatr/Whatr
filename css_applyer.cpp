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
		if (applyToList.size()==0) std::cout << "CSSSelect returned empty vector\n";
		else for (	std::vector<HTMLElement*>::iterator el=applyToList.begin();
					el!=applyToList.end();
					el++)
		{
			(*el)->applyCSSDefaults();
			for (int i=0;i<c->ruleProperties.size();i++)
			{
				CSSProperty name = c->ruleProperties.at(i);
				CSSValue value = c->ruleValues.at(i);
				std::cout << "CSSSelect returned: ";
				std::cout << (*el)->text << "\n";
				(*el)->styleFields.push_back(name);
				(*el)->styleValues.push_back(value);
				switch (name)
				{
			//case TOP: (*el)->cssTop = value; break;
			//case LEFT: (*el)->cssLeft = value; break;
			//case BOTTOM: (*el)->cssBottom = value; break;
			//case RIGHT: (*el)->cssRight = value; break;
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
			//case FONT_STRETCH: (*el)->cssFontStretch = value; break;
			case FONT_WEIGHT: (*el)->cssFontWeight = value.constant; break;
			case FONT_SIZE: (*el)->cssFontSize = value; break;
			case FONT_FAMILY: (*el)->cssFontFamily = value.textValue; break;
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
			//case POSITION: (*el)->cssPosition = value.constant; break;
			case TEXT_ALIGN: (*el)->cssTextAlign = value.constant; break;
			case TEXT_TRANSFORM: (*el)->cssTextTransform = value.constant; break;
			case TEXT_INDENT_LENGTH: (*el)->cssTextIndentLength = value; break;
			case TEXT_INDENT_TYPE: (*el)->cssTextIndentType = value.constant; break;
			case WHITE_SPACE: (*el)->cssWhiteSpace = value.constant; break;
			case WORD_SPACING: (*el)->cssWordSpacing = value; break;
			case TEXT_DECORATION_LINE: (*el)->cssTextDecorationLine = value.constant; break;
			case TEXT_DECORATION_STYLE: (*el)->cssTextDecorationStyle = value.constant; break;
			case TEXT_DECORATION_COLOR: (*el)->cssTextDecorationColor = value.colorValue; break;
			//case BORDER_SPACING: (*el)->cssBorderSpacing = value; break;
					default:
						std::cout << "TODODODODOOOOOOO\n";
					break;
				}
			}
		}
	}
	
	PRINT(Applying CSS done!);
	*applyingCSS = 0;
}
