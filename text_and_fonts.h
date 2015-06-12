/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#ifndef text_and_fonts_included
#define text_and_fonts_included yes

#include "const_str.h"
#include <SDL2/SDL_ttf.h>
#include <vector>

void calculateTextSize(ConstStr text, int availableWidth, int* charWidths, int* charHeights, int& widthOut, int& heightOut);
int loadFont(ConstStr name);
class Font
{
public:
	TTF_Font* gFont;
	int fontCharWidth[256];
	int fontCharHeight[256];
};
extern std::vector<Font> fonts;
void initFontArray();

#endif
