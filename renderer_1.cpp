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
