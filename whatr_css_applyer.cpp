#include <iostream>
#include <vector>
#include <string>

#include "whatr_log_funcs.hpp"
#include "whatr_html_lexer.h"
#include "whatr_html_yaccer.h"
#include "whatr_css_lexer.h"
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
	std::cout << "Applying CSS:\n";
	
	
	PRINT(Applying CSS done!\n);
	*applyingCSS = 0;
}
