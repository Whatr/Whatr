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
#include "renderer_1.h"

void recursiveRenderer1(HTMLElement* el);
void* renderer1ThreadFunc(void* args)
{
	PRINT(renderer1ThreadFunc start);
	
	renderer1Args* r = (renderer1Args*)args;
	
	int* rendering1 = r->rendering1;
	std::vector<HTMLElement*>* HTMLElements = r->HTMLElements;
	
	*rendering1 = 1;
	PRINT(renderer1ThreadFunc has set rendering1=1);
	PRINT(Renderer 1 starting:);
	
	for (	std::vector<HTMLElement*>::iterator h=HTMLElements->begin();
			h!=HTMLElements->end();
			h++)
	{
		if ((*h)->type==0) continue; // Skip text nodes
		recursiveRenderer1(*h);
	}
	
	PRINT(Renderer 1 is done);
	*rendering1 = 0;
}
//TTF_SizeText(font, const char *text, int *w, int *h)
void calculateTextSize(ConstStr text, int availableWidth, int* charWidths, int* charHeights, int& widthOut, int& heightOut)
{
	if (text.length==0)
	{
		widthOut = 0;
		heightOut = 0;
		return;
	}
	ConstStrIterator i = text.iterate();
	int width = 0;
	const int height = charHeights['|'];
	int lines = 0;
	int* lineBreaks = new int[256];
	int lastWhiteSpace = -1;
	for (; i.pos<text.length; i++)
	{
		char c = *i;
		if (c==' ' || c=='\n' || c=='\r' || c=='\t')
		{
			lastWhiteSpace = i.pos;
		}
		width += charWidths[c];
		if (width>availableWidth)
		{
			if (lastWhiteSpace==-1) // cut the word
			{
				lineBreaks[lines] = i.pos-1;
				lines++;
				i--;
				width = 0;
			}
			else // cut the sentence
			{
				i.jump(lastWhiteSpace);
				lineBreaks[lines] = lastWhiteSpace;
				lines++;
				lastWhiteSpace = -1;
				width = 0;
			}
		}
	}
	lineBreaks[lines] = i.pos;
	lines++;
	for (int j=0;j<lines;j++)
	{
		int start = (j==0) ? 0 : lineBreaks[j-1]+1;
		text.subString(start, lineBreaks[j]-start).printLine();
	}
	widthOut = width;
	heightOut = height*lines;
}
void recursiveRenderer1(HTMLElement* el)
{
	std::cout << "recursiveRenderer1 is at a <" << el->text << "...>\n";
	for (	std::vector<HTMLElement*>::iterator h=el->children.begin();
			h!=el->children.end();
			h++)
	{
		if ((*h)->type==0) continue; // Skip text nodes
		recursiveRenderer1(*h);
	}
}
