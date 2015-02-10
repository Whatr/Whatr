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
		else for (std::vector<HTMLElement*>::iterator el=applyToList.begin(); el!=applyToList.end(); el++)
		{
			std::cout << "CSSSelect returned: ";
			std::cout << (*el)->text << "\n";
		}
	}
	
	PRINT(Applying CSS done!);
	*applyingCSS = 0;
}
