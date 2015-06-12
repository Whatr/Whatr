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
