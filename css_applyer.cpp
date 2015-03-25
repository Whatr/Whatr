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
#include "css_yaccer.h"
#include "css_applyer.h"
#include "css_selector.h"
#include "css_values.h"

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
			for (int i=0;i<c->ruleNames.size();i++)
			{
				std::string name = c->ruleNames.at(i);
				CSSValue value = c->ruleValues.at(i);
				std::cout << "CSSSelect returned: ";
				std::cout << (*el)->text << "\n";
				(*el)->styleFields.push_back(name);
				(*el)->styleValues.push_back(value);
				
			}
		}
	}
	
	PRINT(Applying CSS done!);
	*applyingCSS = 0;
}
