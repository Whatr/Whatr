/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#ifndef renderer_1_included
#define renderer_1_included yes

#include "html_yaccer.h"
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class renderer1Args
{
public:
	int* rendering1;
	HTMLElement* document;
	renderer1Args(	int* rendering1,
					HTMLElement* document):
		rendering1(rendering1),
		document(document){};
};
void* renderer1ThreadFunc(void* args);
void calculateTextSize(ConstStr text, int availableWidth, int* charWidths, int* charHeights, int& widthOut, int& heightOut);

#endif
