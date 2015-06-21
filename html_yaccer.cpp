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
				el->css[DISPLAY] = INLINE;
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
				el->applyCSSDefaults();
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
				el->applyCSSDefaults();
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
					el->css[DISPLAY] = INLINE;
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
					el->css[DISPLAY] = INLINE;
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
				el->applyCSSDefaults();
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
				el->applyCSSDefaults();
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
	
	std::cout << "applying css defaults to " << text << "\n";
	
	// Set everything to 0
	memset(&css[0], 0, sizeof(css));
	
	// All non-zero property defaults:
	//css[ALIGN_CONTENT] = STRETCH;
	//css[ALIGN_ITEMS] = STRETCH;
	//css[ALIGN_SELF] = AUTO;
	
	css[BACKGROUND_ATTACHMENT] = SCROLL;
	//css[BACKGROUND_CLIP] = BORDER_BOX;
	css[BACKGROUND_COLOR] = CSSValue(COLOR_TYPE_YUP, 0x000000FF); // transparent
	//css[BACKGROUND_IMAGE] = ;
	//css[BACKGROUND_ORIGIN] = PADDING_BOX;
	css[BACKGROUND_POSITION_X] =
	css[BACKGROUND_POSITION_Y] = CSSValue(LENGTH_TYPE_PERCENT, 0);
	css[BACKGROUND_REPEAT] = REPEAT;
	//css[BACKGROUND_SIZE] = AUTO;
	
	css[BORDER_BOTTOM_WIDTH] = 
	css[BORDER_LEFT_WIDTH] = 
	css[BORDER_RIGHT_WIDTH] = 
	css[BORDER_TOP_WIDTH] = CSSValue(LENGTH_TYPE_PX, 0);
	
	//css[BORDER_COLLAPSE] = SEPARATE;
	
	//css[BORDER_IMAGE_OUTSET] = CSSValue(LENGTH_TYPE_PX, 0);
	//css[BORDER_IMAGE_REPEAT] = STRETCH;
	//css[BORDER_IMAGE_SLICE] = CSSValue(LENGTH_TYPE_PERCENT, 100);
	
	//css[BORDER_IMAGE_WIDTH] = ;
	//css[BORDER_SPACING] = ;
	
	css[BOTTOM] = AUTO;
	//css[CAPTION_SIDE] = TOP_CAPTION_SIDE;
	css[CLEAR] = NONE_CLEAR;
	
	css[CLIP] = AUTO;
	css[COLOR] = CSSValue(COLOR_TYPE_YUP, 0x00000000);
	
	//css[COUNTER_INCREMENT] = NONE_COUNTER_INCREMENT;
	//css[COUNTER_RESET] = NONE_COUNTER_RESET;
	css[CURSOR] = AUTO;
	//css[DIRECTION] = LTR;
	css[DISPLAY] = INLINE;
	//css[EMPTY_CELLS] = SHOW;
	
	css[FLOAT] = NONE_FLOAT;
	
	css[FONT_FAMILY] = NULL;//loadFont("arial.ttf");
	css[FONT_SIZE] = MEDIUM;
	css[FONT_STYLE] = NORMAL_FONT_STYLE;
	css[FONT_VARIANT] = NORMAL_FONT_VARIANT;
	css[FONT_WEIGHT] = NORMAL_FONT_WEIGHT;
	
	css[HEIGHT] = AUTO;
	
	css[LEFT] = AUTO;
	//css[LETTER_SPACING] = NORMAL_LETTER_SPACING;
	//css[LINE_HEIGHT] = NORMAL_LINE_HEIGHT;
	css[LIST_STYLE_POSITION] = OUTSIDE;
	css[LIST_STYLE_TYPE] = DISC;
	
	css[MARGIN_BOTTOM] =
	css[MARGIN_TOP] =
	css[MARGIN_LEFT] =
	css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_PX, 0);
	
	css[MAX_WIDTH] =
	css[MAX_HEIGHT] = CSSValue(LENGTH_TYPE_PX, 99999999);
	css[MIN_WIDTH] =
	css[MIN_HEIGHT] = CSSValue(LENGTH_TYPE_PX, 0);
	
	//css[OUTLINE_COLOR] = ;
	//css[OUTLINE_OFFSET] = ;
	//css[OUTLINE_STYLE] = ;
	//css[OUTLINE_WIDTH] = ;
	//css[OVERFLOW_X] = VISIBLE;
	//css[OVERFLOW_Y] = VISIBLE;
	
	css[PADDING_BOTTOM] =
	css[PADDING_TOP] =
	css[PADDING_LEFT] =
	css[PADDING_RIGHT] = CSSValue(LENGTH_TYPE_PX, 0);
	
	css[POSITION] = RELATIVE; // STATIC?
	
	css[RIGHT] = AUTO;
	
	css[TABLE_LAYOUT] = AUTO;
	css[TEXT_ALIGN] = LEFT_TEXT_ALIGN;
	
	css[TEXT_DECORATION_COLOR] = CSSValue(COLOR_TYPE_YUP, 0x00000000);
	css[TEXT_DECORATION_LINE] = NONE_TEXT_DECORATION;
	css[TEXT_DECORATION_STYLE] = SOLID_TEXT_DEC;
	css[TEXT_INDENT_LENGTH] = CSSValue(LENGTH_TYPE_PX, 0);
	css[TEXT_TRANSFORM] = NONE_TEXT_TRANSFORM;
	css[TOP] = AUTO;
	//css[UNICODE_BIDI] = NORMAL_UNICODE_BIDI; // wtf
	css[VERTICAL_ALIGN] = BASELINE;
	
	css[VISIBILITY] = VISIBLE;
	
	//css[WHITE_SPACE] = NORMAL_WHITE_SPACE;
	css[WIDTH] = AUTO;
	//css[WORD_BREAK] = NORMAL_WORD_BREAK;
	//css[WORD_SPACING] = NORMAL_WORD_SPACING;
	css[Z_INDEX] = AUTO;
	
	// Tag-specific defaults:
	switch (this->tag)
	{
		case TAG_A:
			css[COLOR] = CSSValue(COLOR_TYPE_YUP, 0x0000FF00); // blue
			css[CURSOR] = HAND;
			css[TEXT_DECORATION_LINE] = UNDERLINE;
		break;
		case TAG_ADDRESS:
			css[DISPLAY] = BLOCK;
			css[FONT_STYLE] = ITALIC;
		break;
		case TAG_AREA:
			css[DISPLAY] = NONE_DISPLAY;
		break;
		case TAG_ARTICLE:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_ASIDE:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_B:
			css[FONT_WEIGHT] = BOLD;
		break;
		case TAG_BDO:
			//css[UNICODE_BIDI] = BIDI_OVERRIDE; // idk
		break;
		case TAG_BLOCKQUOTE:
			css[DISPLAY] = BLOCK;
			css[MARGIN_BOTTOM] = css[MARGIN_TOP] = CSSValue(LENGTH_TYPE_EM, 1);
			css[MARGIN_LEFT] = css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_PX, 40);
		break;
		case TAG_BODY:
			std::cout << "TAG_BODY\n";
			css[DISPLAY] = CSSValue(BLOCK);
			//css[MARGIN_TOP] = css[MARGIN_BOTTOM] =
			//css[MARGIN_LEFT] = css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_PX, 8);
			css[PADDING_TOP ] = css[PADDING_BOTTOM] =
			css[PADDING_LEFT] = css[PADDING_RIGHT ] = CSSValue(LENGTH_TYPE_PX, 8);
		break;
		case TAG_CAPTION:
			css[DISPLAY] = TABLE_CAPTION;
			css[TEXT_ALIGN] = CENTER_TEXT_ALIGN;
		break;
		case TAG_CITE:
			css[FONT_STYLE] = ITALIC;
		break;
		case TAG_CODE:
			css[FONT_FAMILY] = NULL;//loadFont("monospace");
		break;
		case TAG_COL:
			css[DISPLAY] = TABLE_COLUMN;
		break;
		case TAG_COLGROUP:
			css[DISPLAY] = TABLE_COLUMN_GROUP;
		break;
		case TAG_DATALIST:
			css[DISPLAY] = NONE_DISPLAY;
		break;
		case TAG_DD:
			css[DISPLAY] = BLOCK;
			css[MARGIN_LEFT] = CSSValue(LENGTH_TYPE_PX, 40);
		break;
		case TAG_DEL:
			css[TEXT_DECORATION_LINE] = LINE_THROUGH;
		break;
		case TAG_DETAILS:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_DFN:
			css[FONT_STYLE] = ITALIC;
		break;
		case TAG_DIV:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_DL:
			css[DISPLAY] = BLOCK;
			css[MARGIN_BOTTOM] =
			css[MARGIN_TOP] = CSSValue(LENGTH_TYPE_EM, 1);
			css[MARGIN_RIGHT] =
			css[MARGIN_LEFT] = CSSValue(LENGTH_TYPE_PX, 0);
		break;
		case TAG_DT:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_EM:
			css[FONT_STYLE] = ITALIC;
		break;
		case TAG_FIELDSET:
			css[DISPLAY] = BLOCK;
			css[MARGIN_RIGHT] =
			css[MARGIN_LEFT] = CSSValue(LENGTH_TYPE_PX, 2);
			css[PADDING_TOP] = CSSValue(LENGTH_TYPE_EM, 0.35);
			css[PADDING_BOTTOM] = CSSValue(LENGTH_TYPE_EM, 0.625);
			css[PADDING_RIGHT] =
			css[PADDING_LEFT] = CSSValue(LENGTH_TYPE_EM, 0.75);
			css[BORDER_LEFT_WIDTH] =
			css[BORDER_RIGHT_WIDTH] =
			css[BORDER_TOP_WIDTH] =
			css[BORDER_BOTTOM_WIDTH] = CSSValue(LENGTH_TYPE_PX, 2);
			css[BORDER_LEFT_STYLE] =
			css[BORDER_RIGHT_STYLE] =
			css[BORDER_TOP_STYLE] =
			css[BORDER_BOTTOM_STYLE] = GROOVE;
			css[BORDER_LEFT_COLOR] =
			css[BORDER_RIGHT_COLOR] =
			css[BORDER_TOP_COLOR] =
			css[BORDER_BOTTOM_COLOR] = CSSValue(COLOR_TYPE_YUP, 0xFF00FF00);
		break;
		case TAG_FIGCAPTION:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_FIGURE:
			css[DISPLAY] = BLOCK;
			css[MARGIN_BOTTOM] =
			css[MARGIN_TOP] = CSSValue(LENGTH_TYPE_EM, 1);
			css[MARGIN_RIGHT] =
			css[MARGIN_LEFT] = CSSValue(LENGTH_TYPE_PX, 40);
		break;
		case TAG_FOOTER:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_FORM:
			css[DISPLAY] = BLOCK;
			css[MARGIN_TOP] = CSSValue(LENGTH_TYPE_EM, 0);
		break;
		case TAG_H1:
			css[DISPLAY] = BLOCK;
			css[FONT_SIZE] = CSSValue(LENGTH_TYPE_EM, 2);
			css[MARGIN_TOP] =
			css[MARGIN_BOTTOM] = CSSValue(LENGTH_TYPE_EM, 0.67);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_EM, 0);
			css[FONT_WEIGHT] = BOLD;
		break;
		case TAG_H2:
			css[DISPLAY] = BLOCK;
			css[FONT_SIZE] = CSSValue(LENGTH_TYPE_EM, 1.5);
			css[MARGIN_TOP] =
			css[MARGIN_BOTTOM] = CSSValue(LENGTH_TYPE_EM, 0.83);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_EM, 0);
			css[FONT_WEIGHT] = BOLD;
		break;
		case TAG_H3:
			css[DISPLAY] = BLOCK;
			css[FONT_SIZE] = CSSValue(LENGTH_TYPE_EM, 1.17);
			css[MARGIN_TOP] =
			css[MARGIN_BOTTOM] = CSSValue(LENGTH_TYPE_EM, 1);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_EM, 0);
			css[FONT_WEIGHT] = BOLD;
		break;
		case TAG_H4:
			css[DISPLAY] = BLOCK;
			css[MARGIN_TOP] =
			css[MARGIN_BOTTOM] = CSSValue(LENGTH_TYPE_EM, 1.33);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_EM, 0);
			css[FONT_WEIGHT] = BOLD;
		break;
		case TAG_H5:
			css[DISPLAY] = BLOCK;
			css[FONT_SIZE] = CSSValue(LENGTH_TYPE_EM, .83);
			css[MARGIN_TOP] =
			css[MARGIN_BOTTOM] = CSSValue(LENGTH_TYPE_EM, 1.67);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_EM, 0);
			css[FONT_WEIGHT] = BOLD;
		break;
		case TAG_H6:
			css[DISPLAY] = BLOCK;
			css[FONT_SIZE] = CSSValue(LENGTH_TYPE_EM, .67);
			css[MARGIN_TOP] =
			css[MARGIN_BOTTOM] = CSSValue(LENGTH_TYPE_EM, 2.33);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_EM, 0);
			css[FONT_WEIGHT] = BOLD;
		break;
		case TAG_HEADER:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_HR:
			css[DISPLAY] = BLOCK;
			css[MARGIN_TOP] =
			css[MARGIN_BOTTOM] = CSSValue(LENGTH_TYPE_EM, 0.5);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = AUTO;
			css[BORDER_STYLE] = INSET;
			css[BORDER_TOP_WIDTH] =
			css[BORDER_BOTTOM_WIDTH] =
			css[BORDER_LEFT_WIDTH] =
			css[BORDER_RIGHT_WIDTH] = CSSValue(LENGTH_TYPE_PX, 1);
		break;
		case TAG_I:
			css[FONT_STYLE] = ITALIC;
		break;
		case TAG_IMG:
			css[DISPLAY] = INLINE_BLOCK;
		break;
		case TAG_INS:
			css[TEXT_DECORATION_LINE] = UNDERLINE;
		break;
		case TAG_KBD:
			css[FONT_FAMILY] = NULL;//loadFont("monospace");
		break;
		case TAG_LABEL:
			css[CURSOR] = DEFAULT_CURSOR;
		break;
		case TAG_LEGEND:
			css[DISPLAY] = BLOCK;
			css[PADDING_LEFT] =
			css[PADDING_RIGHT] = CSSValue(LENGTH_TYPE_PX, 2);
			// border: none;
		break;
		case TAG_LI:
			css[DISPLAY] = LIST_ITEM;
		break;
		case TAG_LINK:
			css[DISPLAY] = NONE_DISPLAY;
		break;
		case TAG_MAP:
			css[DISPLAY] = INLINE;
		break;
		case TAG_MARK:
			css[BACKGROUND_COLOR] = CSSValue(COLOR_TYPE_YUP, 0xFFFF0000);
			css[COLOR] = CSSValue(COLOR_TYPE_YUP, 0x00000000);
		break;
		case TAG_MENU:
			css[DISPLAY] = BLOCK;
			css[LIST_STYLE_TYPE] = DISC;
			css[MARGIN_TOP] =
			css[MARGIN_BOTTOM] = CSSValue(LENGTH_TYPE_EM, 1);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_PX, 0);
			css[PADDING_LEFT] = CSSValue(LENGTH_TYPE_PX, 40);
		break;
		case TAG_NAV:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_NOSCRIPT: // we aint got no scripts ;(
			css[DISPLAY] = INLINE_BLOCK;
		break;
		case TAG_OL:
			css[DISPLAY] = BLOCK;
			css[LIST_STYLE_TYPE] = DECIMAL;
			css[MARGIN_BOTTOM] =
			css[MARGIN_TOP] = CSSValue(LENGTH_TYPE_EM, 1);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_PX, 0);
			css[PADDING_LEFT] = CSSValue(LENGTH_TYPE_PX, 40);
		break;
		case TAG_OUTPUT:
			css[DISPLAY] = INLINE;
		break;
		case TAG_P:
			css[DISPLAY] = BLOCK;
			css[MARGIN_BOTTOM] =
			css[MARGIN_TOP] = CSSValue(LENGTH_TYPE_EM, 1);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_PX, 0);
		break;
		case TAG_PARAM:
			css[DISPLAY] = NONE_DISPLAY;
		break;
		case TAG_PRE:
			css[DISPLAY] = BLOCK;
			css[FONT_FAMILY] = NULL;//loadFont("monospace");
			//css[WHITE_SPACE] = PRE_WHITE_SPACE;
			css[MARGIN_BOTTOM] =
			css[MARGIN_TOP] = CSSValue(LENGTH_TYPE_EM, 1);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_PX, 0);
		break;
		case TAG_Q:
			css[DISPLAY] = INLINE;
			// q:before { content: open-quote ; }
			// q:after  { content: close-quote; }
		break;
		case TAG_RT:
			//css[LINE_HEIGHT] = NORMAL_LINE_HEIGHT;
		break;
		case TAG_S:
			css[TEXT_DECORATION_STYLE] = LINE_THROUGH;
		break;
		case TAG_SAMP:
			css[FONT_FAMILY] = NULL;//loadFont("monospace");
		break;
		case TAG_SCRIPT:
			css[DISPLAY] = NONE_DISPLAY;
		break;
		case TAG_SECTION:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_SMALL:
			css[FONT_SIZE] = SMALLER;
		break;
		case TAG_STRONG:
			css[FONT_WEIGHT] = BOLD;
		break;
		case TAG_STYLE:
			css[DISPLAY] = NONE_DISPLAY;
		break;
		case TAG_SUB:
			css[VERTICAL_ALIGN] = SUB;
			css[FONT_SIZE] = SMALLER;
		break;
		case TAG_SUMMARY:
			css[DISPLAY] = BLOCK;
		break;
		case TAG_SUP:
			css[VERTICAL_ALIGN] = SUPER;
			css[FONT_SIZE] = SMALLER;
		break;
		case TAG_TABLE:
			css[DISPLAY] = TABLE;
			//css[BORDER_COLLAPSE] = SEPERATE;
			css[BORDER_SPACING] = CSSValue(LENGTH_TYPE_PX, 2);
			css[BORDER_LEFT_COLOR] =
			css[BORDER_RIGHT_COLOR] =
			css[BORDER_TOP_COLOR] =
			css[BORDER_BOTTOM_COLOR] = CSSValue(COLOR_TYPE_YUP, 0x80808000); // gray
		break;
		case TAG_TBODY:
			css[DISPLAY] = TABLE_ROW_GROUP;
			css[VERTICAL_ALIGN] = MIDDLE;
			css[BORDER_LEFT_COLOR] =
			css[BORDER_RIGHT_COLOR] =
			css[BORDER_TOP_COLOR] =
			css[BORDER_BOTTOM_COLOR] = INHERIT;
		break;
		case TAG_TD:
			css[DISPLAY] = TABLE_CELL;
			css[VERTICAL_ALIGN] = INHERIT;
		break;
		case TAG_TFOOT:
			css[DISPLAY] = TABLE_FOOTER_GROUP;
			css[VERTICAL_ALIGN] = MIDDLE;
			css[BORDER_LEFT_COLOR] =
			css[BORDER_RIGHT_COLOR] =
			css[BORDER_TOP_COLOR] =
			css[BORDER_BOTTOM_COLOR] = INHERIT;
		break;
		case TAG_TH:
			css[DISPLAY] = TABLE_CELL;
			css[VERTICAL_ALIGN] = INHERIT;
			css[FONT_WEIGHT] = BOLD;
			css[TEXT_ALIGN] = CENTER_TEXT_ALIGN;
		break;
		case TAG_THEAD:
			css[DISPLAY] = TABLE_HEADER_GROUP;
			css[VERTICAL_ALIGN] = MIDDLE;
			css[BORDER_LEFT_COLOR] =
			css[BORDER_RIGHT_COLOR] =
			css[BORDER_TOP_COLOR] =
			css[BORDER_BOTTOM_COLOR] = INHERIT;
		break;
		case TAG_TR:
			css[DISPLAY] = TABLE_ROW;
			css[VERTICAL_ALIGN] = INHERIT;
			css[BORDER_LEFT_COLOR] =
			css[BORDER_RIGHT_COLOR] =
			css[BORDER_TOP_COLOR] =
			css[BORDER_BOTTOM_COLOR] = INHERIT;
		break;
		case TAG_U:
			css[TEXT_DECORATION_STYLE] = UNDERLINE;
		break;
		case TAG_UL:
			css[DISPLAY] = BLOCK;
			css[LIST_STYLE_TYPE] = DISC;
			css[MARGIN_BOTTOM] =
			css[MARGIN_TOP] = CSSValue(LENGTH_TYPE_EM, 1);
			css[MARGIN_LEFT] =
			css[MARGIN_RIGHT] = CSSValue(LENGTH_TYPE_PX, 0);
			css[PADDING_LEFT] = CSSValue(LENGTH_TYPE_PX, 40);
		break;
		case TAG_VAR:
			css[FONT_STYLE] = ITALIC;
		break;
	}
}


