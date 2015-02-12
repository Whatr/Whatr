#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

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
		if (ss->type==0)
		{
			if (op==' ' || op=='>') // Select children
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
			else if (op=='+' || op=='~') // Select siblings
			{
				PRINT(in op+);
				std::vector<HTMLElement*> newVector;
				for (	std::vector<HTMLElement*>::iterator els=selected.begin();
						els!=selected.end();
						els++)
				{
					PRINT(cp 1);
					std::vector<HTMLElement*> addThis = CSSSelectSiblings(*els, *ss, op=='+');
					PRINT(cp 2);
					std::cout << "addThis.size()==" << addThis.size() << "\n";
					PRINT(cp 3);
					newVector.insert(newVector.end(), addThis.begin(), addThis.end());
					PRINT(cp 4);
				}
				selected = std::vector<HTMLElement*>();
				// Add all non-duplicates from newVector to selected
				for (	std::vector<HTMLElement*>::iterator els=newVector.begin();
						els!=newVector.end();
						els++)
				{
					if (std::find(selected.begin(), selected.end(), *els) == selected.end())
					{
						selected.push_back(*els);
					}
				}
			}
			else
			{
				std::cout << RED << "Fatal CSS selector error: inside unknown op='" << op << "'\n" << NOCLR;
			}
		}
		else if (ss->type==-1) // Operator
		{
			if (ss->str1==std::string(" ")) op = ' ';
			else if (ss->str1==std::string(">")) op = '>';
			else if (ss->str1==std::string("+")) op = '+';
			else if (ss->str1==std::string("~")) op = '~';
			else
			{
				std::cout << RED << "TODO: op=" << ss->str1 << "\n" << NOCLR;
			}
		}
		else if (ss->type==10) // *
		{
			selected = std::vector<HTMLElement*>();
			CSSSelectAll(from, &selected);
		}
		else
		{
			std::cout << RED << "TODO: ss type=" << ss->type << " str1=" << ss->str1 << "\n" << NOCLR;
		}
		first = false;
	}
	std::cout << "Exiting CSSSelect(...,...)...\n";
	return selected;
}

void CSSSelectAll(HTMLElement* top, std::vector<HTMLElement*>* dest)
{
	dest->push_back(top);
	for (	std::vector<HTMLElement*>::iterator els=top->children.begin();
						els!=top->children.end();
						els++)
	{
		if ((*els)->type==1) CSSSelectAll(*els, dest);
	}
}
std::vector<HTMLElement*> CSSSelectSiblings(HTMLElement* from, CSSSubSelector ss, bool directlyAfter)
{
	PRINT(Entering CSSSelectSiblings...);
	std::cout << "from=<" << from->text << ">\n";
	std::vector<HTMLElement*> ret;
	bool started = false;
	for (	std::vector<HTMLElement*>::iterator els=from->parent->children.begin();
						els!=from->parent->children.end();
						els++)
	{
		if ((*els)->type==1)
		{
			std::cout << "@ child <" << (*els)->text << ">\n";
			if (!started && *els==from) started = true;
			if (started)
			{
				PRINT(checking applies(..,..)...);
				if (applies(&ss, *els))
				{
					std::cout << "^ Matches!\n";
					ret.push_back(*els);
				}
				if (directlyAfter) break;
			}
		}
	}
	PRINT(exiting CSSSelectSiblings...);
	return ret;
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
		case 1:		// elements with class str1
			
		break;
		case 2:		// elements that have an str1 attribute
			for (	std::vector<std::string>::iterator args=el->argNames.begin();
					args!=el->argNames.end();
					args++)
			{
				if (*args == ss->str1) return true;
			}
			return false;
		break;
		case 3:		// elements that have an str1 attribute and where str1 = str2
			
		break;
		case 4:		//
			
		break;
		case 5:		//
			
		break;
	}
}
