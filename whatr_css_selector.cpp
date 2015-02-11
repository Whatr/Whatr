#include <vector>
#include <string>
#include <iostream>

#include "whatr_log_funcs.hpp"
#include "whatr_html_yaccer.h"
#include "whatr_css_selector.h"

std::vector<HTMLElement*> CSSSelect(HTMLElement* from, CSSSelector* selector)
{
	std::cout << "Entering CSSSelect(...,...)...\n";
	std::vector<HTMLElement*> selected;
	std::cout << "Selecting from <" << from->text << "> elememt:\n";
	
	char op = ' ';
	bool first = true;
	
	selected.push_back(from);
	
	for (std::vector<CSSSubSelector>::iterator ss=selector->subSelectors.begin(); ss!=selector->subSelectors.end(); ss++)
	{
		std::cout << "Subselector: ";
		std::cout << ss->str1 << "\n";
		if (ss->type!=-1)
		{
			if (op==' ' || op=='>') // Select all children and sub-children
			{
				std::vector<HTMLElement*> newVector;
				for (	std::vector<HTMLElement*>::iterator els=selected.begin();
						els!=selected.end();
						els++)
				{
					std::vector<HTMLElement*> addThis = CSSSelect2(*els, *ss, op==' ');
					std::cout << "addThis.size()==" << addThis.size() << "\n";
					newVector.insert(newVector.end(), addThis.begin(), addThis.end());
				}
				selected = newVector;
			}
			else
			{
				std::cout << RED << "Fatal CSS selector error: inside unknown op='" << op << "'\n" << NOCLR;
			}
		}
		else if (ss->str1==std::string(" ")) op = ' ';
		else if (ss->str1==std::string(">")) op = '>';
		else
		{
			std::cout << RED << "TODO: ss type=" << ss->type << " str1=" << ss->str1 << "\n" << NOCLR;
		}
		first = false;
	}
	std::cout << "Exiting CSSSelect(...,...)...\n";
	return selected;
}

std::vector<HTMLElement*> CSSSelect2(HTMLElement* from, CSSSubSelector ss, bool recurse)
{
	//std::cout << "Entering CSSSelect2(...,...)\n";
	std::vector<HTMLElement*> ret;
	for (	std::vector<HTMLElement*>::iterator els=from->children.begin();
						els!=from->children.end();
						els++)
	{
		if ((*els)->type==1)
		{
			std::cout << "@ child <" << (*els)->text << ">\n";
			if (applies(&ss, *els))
			{
				std::cout << "^ Matches! checking its children...\n";
				ret.push_back(*els);
			}
			if (recurse)
			{
				std::vector<HTMLElement*> addThis = CSSSelect2(*els, ss, recurse);
				ret.insert(ret.end(), addThis.begin(), addThis.end());
			}
		}
	}
	//std::cout << "Exiting CSSSelect2(...,...)\n";
	return ret;
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
