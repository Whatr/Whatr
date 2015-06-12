/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

// https://www.youtube.com/watch?v=4uOxOgm5jQ4

#ifndef css_applyer_included
#define css_applyer_included yes

#include "html_yaccer.h"

class cssApplyArgs
{
public:
	int* applyingCSS;
	std::vector<CSSClass>* CSSClasses;
	HTMLElement* destination;
	cssApplyArgs(int* applyingCSS,
				std::vector<CSSClass>* CSSClasses,
				HTMLElement* destination):
		applyingCSS(applyingCSS),
		CSSClasses(CSSClasses),
		destination(destination){};
};
void* cssApplyThreadFunc(void* args);

#endif
