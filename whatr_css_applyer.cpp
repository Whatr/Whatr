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

#include "whatr_log_funcs.hpp"
#include "whatr_html_yaccer.h"
#include "whatr_css_yaccer.h"
#include "whatr_css_applyer.h"
#include "whatr_css_selector.h"

void* cssApplyThreadFunc(void* args)
{
	PRINT(cssApplyThreadFunc start);
	
	cssApplyArgs* c = (cssApplyArgs*)args;
	
	int* applyingCSS = c->applyingCSS;
	std::vector<CSSClass>* CSSClasses = c->CSSClasses;
	HTMLElement* destination = c->destination;
	
	*applyingCSS = 1;
	PRINT(cssApplyThreadFunc has set applyingCss=1);
	PRINT(Applying CSS:);
	
	for (	std::vector<CSSClass>::iterator c=CSSClasses->begin();
			c!=CSSClasses->end();
			c++)
	{
		std::cout << "## ------ Entering a CSS class...\n";
		CSSSelector* selector = &(c->selector);
		std::vector<HTMLElement*> applyToList = CSSSelect(destination, selector);
		if (applyToList.size()==0) std::cout << "CSSSelect returned empty vector\n";
		else for (	std::vector<HTMLElement*>::iterator el=applyToList.begin();
					el!=applyToList.end();
					el++)
		{
			std::cout << "CSSSelect returned: ";
			std::cout << (*el)->text << "\n";
		}
	}
	
	PRINT(Applying CSS done!);
	*applyingCSS = 0;
}
