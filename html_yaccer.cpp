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
#include <algorithm>
#include <vector>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <functional> 
#include <cctype>

#include "log_funcs.hpp"
#include "html_lexer.h"
#include "html_yaccer.h"
#include "css_values.h"

void* htmlYaccThreadFunc(void* args)
{
	PRINT(htmlYaccThreadFunc start);
	htmlYaccArgs* l = (htmlYaccArgs*)args;
	
	int* lexingPage = l->lexingPage;
	int* yaccingPage = l->yaccingPage;
	std::vector<HTMLTag>* HTMLTags = l->HTMLTags;
	HTMLElement* document = l->document;
	std::vector<HTMLElement*>* styles = l->styles;
	std::vector<HTMLElement*>* HTMLElements = &(document->children);
	
	*yaccingPage = 1;
	
	std::string misplacedText = std::string("");
	
	HTMLElement* currentElement = NULL;
	
	int i = 0;
	HTMLTag* currentTag = &((*HTMLTags)[0]);
	while
	(
		*lexingPage
		||
		i<HTMLTags->size()
	)
	{
		while
		(
			i>=HTMLTags->size()
			&&
			*lexingPage==1
		)
		{
			PRINT(HTML Yacc thread is waiting for lexer...);
			usleep(100000);
		}
		if
		(
			i>=HTMLTags->size()
			&&
			*lexingPage==0
		)
		{
			PRINT(HTML Yacc thread detected Lexer=done.);
			break;
		}
		
		//std::cout << "yacc:currentTag=" << currentTag->text << "\n";
		
		if (currentTag->type==1)
		{
			// The following tags are always self-closing:
			if (currentTag->tag > _TAG_SELF_CLOSING_START &&
				currentTag->tag < _TAG_SELF_CLOSING_END)
			{
				currentTag->type = 2;
			}
		}
		// The following tags are deprecated in HTML5
		if (currentTag->tag > _TAG_DEPRECATED_START &&
			currentTag->tag < _TAG_DEPRECATED_END)
		{
			std::cout << RED << "Yacc warning: The <" << currentTag->text << "> tag is deprecated in HTML5.\n" << NOCLR;
		}
		
		//---------Begin of the actual yaccing
		if (currentElement==NULL)
		{
			if (currentTag->type==0) // Text node
			{
				HTMLElement* el = new HTMLElement();
				el->type = 0;
				el->text = currentTag->text;
				el->parent = document;
				HTMLElements->push_back(el);
			}
			else if (currentTag->type==1) // Opening tag
			{
				HTMLElement* el = new HTMLElement();
				el->type = 1;
				el->text = currentTag->text;
				el->tag = currentTag->tag;
				el->parent = document;
				el->argNames = currentTag->argNames;
				el->argValues = currentTag->argValues;
				HTMLElements->push_back(el);
				currentElement = HTMLElements->back();
			}
			else if (currentTag->type==2) // Opening & self-closing tag
			{
				HTMLElement* el = new HTMLElement();
				el->type = 1;
				el->text = currentTag->text;
				el->tag = currentTag->tag;
				el->parent = document;
				el->argNames = currentTag->argNames;
				el->argValues = currentTag->argValues;
				HTMLElements->push_back(el);
			}
			else if (currentTag->type==3) // Closing tag
			{
				std::cout << RED << "Yacc error: Unexpected closing tag </";
				currentTag->text.print();
				std::cout << NOCLR << ">, there are no tags to close.\n";
			}
			else
			{
				std::cout << RED << "Yacc error: Unknown tag type " << currentTag->type << NOCLR << "\n";
			}
		}
		else
		{
			if (currentElement->tag == TAG_BODY &&
				currentElement->children.size() == 0)
			{
				if (misplacedText.length()!=0)
				{
					std::cout << RED << "Yacc warning: Added misplaced text on top of <body> contents\n";
					HTMLElement* el = new HTMLElement();
					el->type = 0;
					el->text = misplacedText;
					el->parent = currentElement;
					currentElement->children.push_back(el);
				}
			}
			if (currentTag->type==0) // Text node
			{
				if (currentElement->tag == TAG_HTML ||
					currentElement->tag == TAG_HEAD)
				{
					// If text has accidentally been placed directly inside the
					// <head> or directly inside the <html>, we store it in
					// [std::string misplacedText] and later add it at the top of
					// the <body>
					ConstStr tttttt = currentTag->text.trim(' ', '\t', '\n', '\r');
					if (tttttt.length!=0)
					{
						std::cout << RED << "TODO TODO TODO TODO\n";
						//std::cout << RED << "Yacc error: Misplaced text \""<<tttttt<<"\"\n";
						//misplacedText += tttttt + "\n";
					}
				}
				else
				{
					HTMLElement* el = new HTMLElement();
					el->type = 0;
					el->text = currentTag->text;
					el->parent = currentElement;
					currentElement->children.push_back(el);
				}
			}
			else if (currentTag->type==1) // Opening tag
			{
				//std::cout << "Opening tag <" << currentTag->text << ">\n";
				HTMLElement* el = new HTMLElement();
				el->type = 1;
				el->text = currentTag->text;
				el->tag = currentTag->tag;
				el->parent = currentElement;
				el->argNames = currentTag->argNames;
				el->argValues = currentTag->argValues;
				currentElement->children.push_back(el);
				currentElement = el;
			}
			else if (currentTag->type==2) // Opening & self-closing tag
			{
				//std::cout << "Opening & self-closing tag <" << currentTag->text << "/>\n";
				HTMLElement* el = new HTMLElement();
				el->type = 1;
				el->text = currentTag->text;
				el->tag = currentTag->tag;
				el->parent = currentElement;
				el->argNames = currentTag->argNames;
				el->argValues = currentTag->argValues;
				currentElement->children.push_back(el);
			}
			else if (currentTag->type==3) // Closing tag
			{
				if (currentElement->tag!=currentTag->tag)
				{
					int found = 0;
					HTMLElement* checkingElement = currentElement;
					while (checkingElement!=NULL)
					{
						checkingElement = checkingElement->parent;
						if (checkingElement==NULL) break;
						if (checkingElement->tag==currentTag->tag)
						{
							found = 1;
							break;
						}
					}
					if (!found)
					{
						std::cout << RED << "Yacc error: Encountered unmatched closing tag </";
						currentTag->text.print();
						std::cout << "> inside <";
						currentElement->text.print();
						std::cout << "> tag. Ignoring it.\n" << NOCLR;
					}
					else
					{
						std::cout << RED << "Yacc error: Unexpected closing tag </";
						currentTag->text.print();
						std::cout << ">! The following closing tags were not there but have been assumed: ";
						checkingElement = currentElement;
						while (checkingElement!=NULL)
						{
							if (checkingElement->tag==currentTag->tag)
							{
								found = 1;
								break;
							}
							else
							{
								std::cout << "</" << checkingElement->text.copy() << "> ";
							}
							checkingElement = checkingElement->parent;
						}
						currentElement = checkingElement->parent;
						std::cout << NOCLR << "\n";
					}
				}
				else
				{
					if (currentElement->tag == TAG_STYLE)
					{
						styles->push_back(currentElement);
					}
					currentElement = currentElement->parent;
				}
			}
			else
			{
				std::cout << RED << "Yacc error: Unknown tag type " << currentTag->type << NOCLR << "\n";
			}
		}
		
		//---------End of the actual yaccing
		i++;
		currentTag++;
	}
	while (currentElement!=document)
	{
		std::cout << RED << "Yacc error: Unclosed <" << currentElement->text.copy() << "> tag! \n" << NOCLR;
		currentElement = currentElement->parent;
	}
	*yaccingPage = 0;
	PRINT(htmlYaccThreadFunc end);
	pthread_exit(NULL);
}
void HTMLElement::applyCSSDefaults()
{ // http://www.w3.org/TR/CSS2/sample.html
	cssTop.lengthType =
	cssBottom.lengthType =
	cssLeft.lengthType =
	cssRight.lengthType = LENGTH_TYPE_PX;
	cssTop.lengthValue =
	cssBottom.lengthValue =
	cssLeft.lengthValue =
	cssRight.lengthValue = 0;
	cssColor = 0x00000000; // black
	cssBackgroundColor = 0xFFFFFF00; // white
	cssBackgroundAttachment = SCROLL;
	cssBackgroundRepeat = REPEAT;
	memset(&cssBackgroundPositionX, 0, sizeof(CSSValue));
	memset(&cssBackgroundPositionY, 0, sizeof(CSSValue));
	cssBackgroundImage = ConstStr();
	cssBorderBottomColor =
	cssBorderTopColor =
	cssBorderLeftColor =
	cssBorderRightColor = 0xFFFFFF00; // white
	cssBorderBottomStyle =
	cssBorderTopStyle =
	cssBorderLeftStyle =
	cssBorderRightStyle = NONE_LINE_STYLE;
	cssClear = NONE_CLEAR;
	cssDisplay = INLINE;
	cssFloat = NONE_FLOAT;
	cssFontVariant = NORMAL_FONT_VARIANT;
	cssFontStyle = NORMAL_FONT_STYLE;
	cssFontWeight = NORMAL_FONT_WEIGHT;
	cssFontSize.lengthType = LENGTH_TYPE_PT;
	cssFontSize.lengthValue = 12;
	cssWidth.lengthType = cssHeight.lengthType = LENGTH_TYPE_NOPE;
	cssPaddingBottom.lengthType =
	cssPaddingTop.lengthType =
	cssPaddingLeft.lengthType =
	cssPaddingRight.lengthType = LENGTH_TYPE_PX;
	cssPaddingBottom.lengthValue =
	cssPaddingTop.lengthValue =
	cssPaddingLeft.lengthValue =
	cssPaddingRight.lengthValue = 5;
	cssVerticalAlign = TOP_VALIGN;
	cssMarginBottom.lengthType =
	cssMarginTop.lengthType =
	cssMarginLeft.lengthType =
	cssMarginRight.lengthType = LENGTH_TYPE_PX;
	cssMarginBottom.lengthValue =
	cssMarginTop.lengthValue =
	cssMarginLeft.lengthValue =
	cssMarginRight.lengthValue = 0;
	cssTextAlign = START_TEXT_ALIGN;
	cssTextTransform = NONE_TEXT_TRANSFORM;
	cssTextDecorationLine = NONE_TEXT_DECORATION;
	cssDisplay = INLINE_BLOCK;
	
	
	switch (this->tag)
	{
		case TAG_HTML:
		case TAG_ADDRESS:
		case TAG_BLOCKQUOTE:
		case TAG_BODY:
		case TAG_DD:
		case TAG_DIV:
		case TAG_DL:
		case TAG_DT:
		case TAG_FIELDSET:
		case TAG_FORM:
		case TAG_FRAME:
		case TAG_FRAMESET:
		case TAG_H1:
		case TAG_H2:
		case TAG_H3:
		case TAG_H4:
		case TAG_H5:
		case TAG_H6:
		case TAG_NOFRAMES:
		case TAG_OL:
		case TAG_P:
		case TAG_UL:
		case TAG_CENTER:
		case TAG_DIR:
		case TAG_HR:
		case TAG_MENU:
		case TAG_PRE:
			cssDisplay = BLOCK;
		break;
	}
	switch (this->tag)
	{
		case TAG_LI:		cssDisplay = LIST_ITEM; break;
		case TAG_HEAD:		cssDisplay = NONE_DISPLAY; break;
		case TAG_TABLE:		cssDisplay = TABLE; break;
		case TAG_TR:		cssDisplay = TABLE_ROW; break;
		case TAG_THEAD:		cssDisplay = TABLE_HEADER_GROUP; break;
		case TAG_TBODY:		cssDisplay = TABLE_ROW_GROUP; break;
		case TAG_TFOOT:		cssDisplay = TABLE_FOOTER_GROUP; break;
		case TAG_COL:		cssDisplay = TABLE_COLUMN; break;
		case TAG_COLGROUP:	cssDisplay = TABLE_COLUMN_GROUP; break;
		case TAG_TD: case TAG_TH: cssDisplay = TABLE_CELL; break;
		case TAG_CAPTION:	cssDisplay = TABLE_CAPTION; break;
	}
	switch (this->tag)
	{
		case TAG_BODY:
			cssMarginTop.lengthType = LENGTH_TYPE_PX;
			cssMarginTop.lengthValue = 8;
			cssMarginLeft = cssMarginBottom = cssMarginRight = cssMarginTop;
		break;
		case TAG_H1:
			cssFontSize.lengthValue = 2;
			cssMarginTop.lengthValue = .67;
		break;
		case TAG_H2:
			cssFontSize.lengthValue = 1.5;
			cssMarginTop.lengthValue = .75;
		break;
		case TAG_H3:
			cssFontSize.lengthValue = 1.17;
			cssMarginTop.lengthValue = .83;
		break;
		case TAG_H4:
		case TAG_P:
		case TAG_BLOCKQUOTE:
		case TAG_UL:
		case TAG_FIELDSET:
		case TAG_FORM:
		case TAG_OL:
		case TAG_DL:
		case TAG_DIR:
		case TAG_MENU:
			cssMarginTop.lengthType = LENGTH_TYPE_EM;
			cssMarginTop.lengthValue = 1.12;
			cssMarginBottom = cssMarginTop;
			cssMarginLeft.lengthType = LENGTH_TYPE_EM;
			cssMarginLeft.lengthValue = 0;
			cssMarginRight = cssMarginLeft;
		break;
		case TAG_H5:
			cssFontSize.lengthValue = .83;
			cssMarginTop.lengthValue = 1.5;
		break;
		case TAG_H6:
			cssFontSize.lengthValue = .75;
			cssMarginTop.lengthValue = 1.67;
		break;
		case TAG_STRONG: cssFontWeight = BOLDER; break;
	}
	switch (this->tag)
	{
		case TAG_BLOCKQUOTE:
			cssMarginLeft.lengthType = LENGTH_TYPE_PX;
			cssMarginLeft.lengthValue = 40;
			cssMarginRight = cssMarginLeft;
		break;
		case TAG_I:
		case TAG_CITE:
		case TAG_EM:
		case TAG_VAR:
		case TAG_ADDRESS:
			cssFontStyle = ITALIC;
		break;
		case TAG_KBD:
		case TAG_SAMP:
			cssFontFamily = loadFont(ConstStr("monospace")); // TODO
		break;
		case TAG_PRE: cssWhiteSpace = PRE; break;
		case TAG_BUTTON:
		case TAG_TEXTAREA:
		case TAG_INPUT:
		case TAG_SELECT:
			cssDisplay = INLINE_BLOCK;
		break;
		case TAG_BIG:
			cssFontSize.lengthType = LENGTH_TYPE_EM;
			cssFontSize.lengthValue = 1.17;
		break;
		case TAG_SUB:
			cssVerticalAlign = SUB;
			cssFontSize.lengthType = LENGTH_TYPE_EM;
			cssFontSize.lengthValue = .83;
		break;
		case TAG_SUP:
			cssVerticalAlign = SUPER;
			cssFontSize.lengthType = LENGTH_TYPE_EM;
			cssFontSize.lengthValue = .83;
		break;
		case TAG_SMALL:
			cssFontSize.lengthType = LENGTH_TYPE_EM;
			cssFontSize.lengthValue = .83;
		break;
		case TAG_TABLE:
			cssBorderSpacing.lengthType = LENGTH_TYPE_PX;
			cssBorderSpacing.lengthValue = 2;
		break;
		case TAG_THEAD:
		case TAG_TBODY:
		case TAG_TFOOT:
			cssVerticalAlign = MIDDLE;
		break;
		case TAG_TD:
		case TAG_TH:
		case TAG_TR:
			cssVerticalAlign = INHERIT;
		break;
		case TAG_S:
		case TAG_STRIKE:
		case TAG_DEL:
			cssTextDecorationLine = LINE_THROUGH;
		break;
		case TAG_HR:
			cssBorderLeftWidth.lengthType = LENGTH_TYPE_PX;
			cssBorderLeftWidth.lengthValue = 1;
			cssBorderRightWidth = cssBorderTopWidth = cssBorderBottomWidth = cssBorderLeftWidth;
			cssBorderLeftStyle = cssBorderRightStyle = cssBorderTopStyle = cssBorderBottomStyle = INSET;
		break;
	}
	switch (this->tag)
	{
		case TAG_OL:
			cssListStyleType = DECIMAL;
		// fall through
		case TAG_UL:
		case TAG_DIR:
		case TAG_MENU:
		case TAG_DD:
			cssMarginLeft.lengthType = LENGTH_TYPE_PX;
			cssMarginLeft.lengthValue = 40;
		break;
		case TAG_U:
		case TAG_INS:
			cssTextDecorationLine = UNDERLINE;
		break;
	}
	switch (this->tag)
	{
		case TAG_H1:
		case TAG_H2:
		case TAG_H3:
		case TAG_H4:
		case TAG_H5:
		case TAG_H6:
			cssFontSize.lengthType = LENGTH_TYPE_EM;
			cssMarginTop.lengthType = LENGTH_TYPE_EM;
			cssMarginBottom = cssMarginTop;
			cssMarginLeft.lengthType = LENGTH_TYPE_EM;
			cssMarginLeft.lengthValue = 0;
			cssMarginRight = cssMarginLeft;
		break;
	}
}


