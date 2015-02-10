#include <vector>
#include <string>
#include <iostream>

#include "whatr_html_yaccer.h"
#include "whatr_css_selector.h"

std::vector<HTMLElement*> CSSSelect(HTMLElement* from, CSSSelector* selector)
{
	std::cout << "Entering CSSSelect(...,...)...\n";
	std::vector<HTMLElement*> selected;
	std::cout << "Selecting from <" << from->text << "> elememt:\n";
	
	for (std::vector<CSSSubSelector>::iterator ss=selector->subSelectors.begin(); ss!=selector->subSelectors.end(); ss++)
	{
		std::cout << "Subselector: ";
		std::cout << ss->str1 << "\n";
		if (
	}
	std::cout << "Exiting CSSSelect(...,...)...\n";
	return selected;
}

// Check if subselector applies to element:
bool applies(CSSSubSelector* ss, HTMLElement* el)
{
	switch (ss->type)
	{
		case 0:		// str1 elements
			return el->text==ss->str1;
		break;
		case 1:		//
			
		break;
		case 2:		//
			
		break;
		case 3:		//
			
		break;
		case 4:		//
			
		break;
		case 5:		//
			
		break;
	}
}
