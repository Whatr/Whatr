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
#include "renderer_1.h"
#include "text_and_fonts.h"

void recursiveRenderer1(HTMLElement* el, bool insideBody);
void* renderer1ThreadFunc(void* args)
{
	PRINT(renderer1ThreadFunc start);
	
	renderer1Args* r = (renderer1Args*)args;
	
	int* rendering1 = r->rendering1;
	HTMLElement* document = r->document;
	
	*rendering1 = 1;
	PRINT(renderer1ThreadFunc has set rendering1=1);
	PRINT(Renderer 1 starting:);
	
	recursiveRenderer1(document, false);
	
	PRINT(Renderer 1 is done);
	*rendering1 = 0;
}
void recursiveRenderer1(HTMLElement* el, bool insideBody)
{
	std::cout << "recursiveRenderer1 is at a <" << el->text;
	if (insideBody) std::cout << " insideBody";
	std::cout << "...>\n";
	if (el->type==0) // Text node
	{
		if (!insideBody) return;
		int widthOut = 0, heightOut = 0, newLinesOut = 0, newHeightOut = 0;
		std::cout << "Text size of:\n" << el->text << "\ncalculated: \n";
		calculateTextSize(el->text, el->parent->availableWidth, fonts.at(0), el->parent->curInlineX, widthOut, heightOut, newLinesOut, newHeightOut);
		std::cout << "widthOut=" << widthOut << "\n";
		std::cout << "heightOut=" << heightOut << "\n";
		std::cout << "newLinesOut=" << newLinesOut << "\n";
		std::cout << "newHeightOut=" << newHeightOut << "\n";
		el->myInlineX = el->parent->curInlineX;
		el->myInlineY = el->parent->curInlineY;
		el->width = -1;
		el->height = heightOut;
		el->parent->curInlineX = widthOut;
		el->parent->curInlineY += newHeightOut;
	}
	else // HTML element
	{
		if (el->tag==TAG_HTML || el->tag==TAG_BODY)
		{
			std::cout << "el->tag==HTML of BODY\n";
			std::cout << "padding_left=" << el->css[PADDING_LEFT].lengthValue << "\n";
			el->availableWidth = el->parent->availableWidth;
		}
		for (	std::vector<HTMLElement*>::iterator h=el->children.begin();
				h!=el->children.end();
				h++)
		{
			if (insideBody)
			{
			}
			recursiveRenderer1(*h, (el->tag == TAG_BODY) || insideBody);
		}
	}
}
