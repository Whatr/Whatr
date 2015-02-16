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

// https://www.youtube.com/watch?v=4uOxOgm5jQ4

#ifndef css_applyer_included
#define css_applyer_included yes

#include "whatr_html_yaccer.h"

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
class cssValue
{
public:
	int type;
	/* 0 = pixels						int
	 * 1 = em							float
	 * 2 = ex							float
	 * 3 = mm (millimeters)				float
	 * 4 = cm (centimeters)				float
	 * 5 = pt (1 points = 1/72 inches)	int
	 * 6 = inches						float
	 * 
	 * 7 = colour						
	 * 
	 * 
	 * 
	 */
	union
	{
		int i;
		float f;
	};
	int i2;
	int i3;
};
void* cssApplyThreadFunc(void* args);

#endif
