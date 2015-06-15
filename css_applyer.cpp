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
		std::cout << "cp1\n";
		if (applyToList.size()==0);// std::cout << "CSSSelect returned empty vector\n";
		else for (	std::vector<HTMLElement*>::iterator el=applyToList.begin();
					el!=applyToList.end();
					el++)
		{
			std::cout << "cp2\n";
			(*el)->applyCSSDefaults();
			std::cout << "cp3\n";
			for (int i=0;i<c->ruleProperties.size();i++)
			{
				CSSProperty name = c->ruleProperties.at(i);
				CSSValue value = c->ruleValues.at(i);
				//std::cout << "CSSSelect returned: ";
				//std::cout << (*el)->text << "\n";
				(*el)->styleFields.push_back(name);
				(*el)->styleValues.push_back(value);
				if (name>=COMBOS)
				{
					std::cout << RED << "CSS error: a combo property was received by css_applyer.cpp!!!\n" << NOCLR;
				}
				else
				{
					(*el)->css[name] = value;
				}
			}
		}
	}
	
	PRINT(Applying CSS done!);
	*applyingCSS = 0;
}
