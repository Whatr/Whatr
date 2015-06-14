/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#include "text_and_fonts.h"
#include "log_funcs.hpp"
#include <iostream>

void initFontArray()
{
	loadFont(ConstStr("arial.ttf"));
}
std::vector<Font*> fonts;
Font* loadFont(ConstStr name)
{
	char* name2 = name.copy();
	TTF_Font* gFont = TTF_OpenFont(name2, 24);
	delete name2;
	if (gFont==NULL)
	{
		std::cout <<RED<< "Loading font " << name << " failed miserably.\n"<<NOCLR;
		return NULL;
	}
	Font* font = new Font();
	font->gFont = gFont;
	char allChars[512];
	for (int i=0;i<256;i++)
	{
		allChars[i*2] = (char)i;
		allChars[i*2+1] = 0;
	}
	
	char* c = &allChars[0];
	for (int i=0;i<256;i++,c+=2)
	{
		if (isprint(*c))
		{
			TTF_SizeText(gFont, c, &(font->fontCharWidth[i]), &(font->fontCharHeight[i]));
		}
		else
		{
			font->fontCharWidth[i] = font->fontCharHeight[i] = 0;
		}
	}
	fonts.push_back(font);
	return font;
}
void calculateTextSize(ConstStr text, int availableWidth, Font* font, int& widthOut, int& heightOut)
{
	calculateTextSize(text, availableWidth, font->fontCharWidth, font->fontCharHeight, widthOut, heightOut);
	
}
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
				std::cout << "wc @"<<*i<<"\n";
				lineBreaks[lines] = i.pos-1;
				lines++;
				i--;
				width = 0;
			}
			else // cut the sentence
			{
				i.jump(lastWhiteSpace);
				lineBreaks[lines] = lastWhiteSpace + 1;
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
		int start = (j==0) ? 0 : lineBreaks[j-1];
		text.subString(start, lineBreaks[j]-start).printLine();
	}
	widthOut = width;
	heightOut = height*lines;
}
