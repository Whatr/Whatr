#include <iostream>
#include <vector>
#include <string>

#include "whatr_log_funcs.hpp"
#include "whatr_html_yaccer.h"
#include "whatr_css_yaccer.h"
#include "whatr_css_applyer.h"

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
		// CSSClass* c = ...;
		CSSSelector* s = &(c->selector);
		for (std::vector<CSSSubSelector>::iterator ss=s->subSelectors.begin(); ss!=s->subSelectors.end(); ss++)
		{
			std::cout << ss->str1 << "\n";
		}
	}
	
	PRINT(Applying CSS done!);
	*applyingCSS = 0;
}
