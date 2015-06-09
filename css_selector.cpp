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

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "log_funcs.hpp"
#include "html_yaccer.h"
#include "css_selector.h"
#include "const_str.h"

std::vector<HTMLElement*> CSSSelect(HTMLElement* from, CSSSelector* selector)
{
	std::vector<std::vector<HTMLElement*>*> selections;
	std::cout << "Entering CSSSelect(...,...)...\n";
	std::vector<HTMLElement*>* selected = new std::vector<HTMLElement*>;
	std::cout << "Selecting from <" << from->text << "> elememt:\n";
	
	char op = ' ';
	bool first = true;
	
	selected->push_back(from);
	
	for (std::vector<CSSSubSelector>::iterator ss=selector->subSelectors.begin(); ss!=selector->subSelectors.end(); ss++)
	{
		if (ss->type == -1)
		{
			std::cout << "ss->type=-1 & op=" << op << "\n";
			if (ss->str1==' ') op = ' ';
			else if (ss->str1=='>') op = '>';
			else if (ss->str1=='+') op = '+';
			else if (ss->str1=='~') op = '~';
			else if (ss->str1==',')
			{
				selections.push_back(selected);
				selected = new std::vector<HTMLElement*>;
				selected->push_back(from);
				op = ' ';
				first = true;
				continue;
			}
			else
			{
				std::cout << RED << "TODO: op=" << ss->str1 << "\n" << NOCLR;
			}
		}
		else if (ss->type == 10) // *
		{
			std::cout << "ss->type=10\n";
			/*
			Select all elements. Every single one.
			*/
			selected = new std::vector<HTMLElement*>;
			CSSSelectAll(from, selected);
			op = '\0';
		}
		else if (ss->type == 0)
		{
			/*
			Delete current selection and make a new one based upon it
			*/
			if (op==' ' || op=='>') // Select children
			{
				std::cout << "Selecting children\n";
				std::vector<HTMLElement*>* newVector = new std::vector<HTMLElement*>;
				for (	std::vector<HTMLElement*>::iterator els=selected->begin();
						els!=selected->end();
						els++)
				{
					std::vector<HTMLElement*> addThis = CSSSelect2(*els, *ss, op==' ');
					newVector->insert(newVector->end(), addThis.begin(), addThis.end());
				}
				selected = newVector;
			}
			else if (op=='+' || op=='~') // Select siblings
			{
				std::cout << "Selecting siblings\n";
				std::vector<HTMLElement*>* newVector = new std::vector<HTMLElement*>;
				for (	std::vector<HTMLElement*>::iterator els=selected->begin();
						els!=selected->end();
						els++)
				{
					std::vector<HTMLElement*> addThis = CSSSelectSiblings(*els, *ss, op=='+');
					newVector->insert(newVector->end(), addThis.begin(), addThis.end());
				}
				selected = new std::vector<HTMLElement*>;
				// Add all non-duplicates from newVector to selected
				for (	std::vector<HTMLElement*>::iterator els=newVector->begin();
						els!=newVector->end();
						els++)
				{
					if (std::find(selected->begin(), selected->end(), *els) == selected->end())
					{
						selected->push_back(*els);
					}
				}
			}
			else
			{
				if (op!='\0') std::cout << RED << "Fatal CSS selector error: inside unknown op='" << op << "'\n" << NOCLR;
				else std::cout << RED << "Fatal CSS selector error: inside zero op!\n" << NOCLR; // TODO handle zero op correctly
			}
			op = '\0';
		}
		else if (op=='\0')
		{
			/*
			Use ss to select elements from the current selection that match it
			*/
			std::vector<HTMLElement*>* newVector = new std::vector<HTMLElement*>;
			for (	std::vector<HTMLElement*>::iterator els=selected->begin();
					els!=selected->end();
					els++)
			{
				if (applies(&*ss, *els)) newVector->push_back(*els);
			}
			selected = newVector;
		}
		else
		{
			/*
			Select children of elements in the current selection that match ss
			*/
			std::vector<HTMLElement*>* newVector = new std::vector<HTMLElement*>;
			for (	std::vector<HTMLElement*>::iterator els=selected->begin();
					els!=selected->end();
					els++)
			{
				std::vector<HTMLElement*> them = CSSSelect2(&**els, *ss, true);
				std::copy (them.begin(), them.end(), std::back_inserter(*newVector));
			}
			selected = newVector;
		}
		first = false;
	}
	std::cout << "Exiting CSSSelect(...,...)...\n";
	for (std::vector<std::vector<HTMLElement*>*>::iterator els=selections.begin();
		 els!=selections.end();
		 els++)
		std::copy ((*els)->begin(), (*els)->end(), std::back_inserter(*selected));
	
	return *selected;
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
	if (from->parent==NULL) return ret; // The <document> has no siblings
	bool started = false;
	for (	std::vector<HTMLElement*>::iterator els=from->parent->children.begin();
						els!=from->parent->children.end();
						els++)
	{
		std::cout << "loop\n";
		if ((*els)->type==1)
		{
			std::cout << "@ child <" << (*els)->text << ">\n";
			if (!started && *els==from) started = true;
			else if (started)
			{
				PRINT(checking applies(..,..)...);
				if (applies(&ss, *els))
				{
					//std::cout << "^ Matches!\n";
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
			//std::cout << "@ child <" << (*els)->text << ">\n";
			if (applies(&ss, *els))
			{
				//std::cout << "^ Matches! checking its children...\n";
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
		{
			return el->text==ss->str1;
		}
		break;
		case 1:		// elements with class str1
			
		break;
		case 2:		// elements that have an str1 attribute
		{
			for (	std::vector<ConstStr>::iterator args=el->argNames.begin();
					args!=el->argNames.end();
					args++)
			{
				if (*args == ss->str1) return true;
			}
			return false;
		}
		break;
		case 3:		// elements that have an str1 attribute and where str1 = str2
		{
			std::vector<ConstStr>::iterator args = el->argNames .begin();
			std::vector<ConstStr>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1) return *vals == ss->str2;
			}
			return false;
		break;
		}
		case 4:		// ~=		elements having a str1 attribute and where str1 contains str2 as part of a space-seperated list
		{
			std::vector<ConstStr>::iterator args = el->argNames .begin();
			std::vector<ConstStr>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1)
				{
					int foundLoc = vals->find(ss->str2);
					if (foundLoc == -1)
					{
						// arg="a b c"
						// checking for "qwertyuiop"
						return false;
					}
					if (vals->length == ss->str2.length)
					{
						// arg="a"
						// checking for "a"
						return true;
					}
					if (foundLoc==0)
					{
						// arg="a b c"
						// checking for "a"
						return (*vals)[ss->str2.length]==' ';
					}
					if (
							(*vals)[foundLoc-1]==' '
							&&
							(
								foundLoc+ss->str2.length==vals->length
								||
								(*vals)[foundLoc+ss->str2.length]==' '
							)
						)
					{
						// arg="a b c"
						// checking for "c" or "b"
						return true;
					}
					return false;
				}
			}
			return false;
		}
		break;
		case 5:		// |=		elements having a str1 attribute and where str1 starts with str2 as part of a hyphen-seperated list
		{
			std::vector<ConstStr>::iterator args = el->argNames .begin();
			std::vector<ConstStr>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1)
				{
					if (vals->length < ss->str2.length) return false;
					return
						(
							vals->subString(0, ss->str2.length) == ss->str2
							&&
							(
								vals->length == ss->str2.length
								||
								(*vals)[ss->str2.length] == '-'
							)
						);
				}
			}
			return false;
		}
		break;
		case 6:		// ^=		elements having a str1 attribute and where str1 starts with str2
		{
			std::vector<ConstStr>::iterator args = el->argNames .begin();
			std::vector<ConstStr>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1)
				{
					if (vals->length < ss->str2.length) return false;
					return vals->subString(0, ss->str2.length) == ss->str2;
				}
			}
			return false;
		}
		break;
		case 11:	// $=		elements having a str1 attribute and where str1 ends with str2
		{
			std::vector<ConstStr>::iterator args = el->argNames .begin();
			std::vector<ConstStr>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1)
				{
					if (vals->length < ss->str2.length) return false;
					return vals->subString(vals->length - ss->str2.length, ss->str2.length) == ss->str2;
				}
			}
			return false;
		}
		break;
		case 12:	// *=		elements having a str1 attribute and where str1 contains str2
		{
			std::vector<ConstStr>::iterator args = el->argNames .begin();
			std::vector<ConstStr>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1)
				{
					return vals->find(ss->str2) != -1;
				}
			}
			return false;
		}
		break;
		case 7:		// Things like :first-child
		{
			if (ss->str1==std::string("first-child"))
			{
				if (el->parent==NULL) return false;
				std::vector<HTMLElement*>::iterator siblings = el->parent->children.begin();
				for (;	siblings!=el->parent->children.end();
						siblings++)
				{
					if ((*siblings)->type==1)
					{
						return el == *siblings;
					}
				}
				std::cout << RED << "Fatal error: aksdfklajselkrmjakls\n" << NOCLR;
			}
			else if (ss->str1==std::string("last-child"))
			{
				if (el->parent==NULL) return false;
				std::vector<HTMLElement*>::iterator siblings = el->parent->children.end();
				siblings--;
				for (;	siblings>=el->parent->children.begin();
						siblings--)
				{
					if ((*siblings)->type==1)
					{
						return el == *siblings;
					}
				}
				std::cout << RED << "Fatal error: lkqwelkqwnekqmwkemqk\n" << NOCLR;
			}
			else if (ss->str1==std::string("only-child"))
			{
				if (el->parent==NULL) return false;
				std::vector<HTMLElement*>::iterator siblings = el->parent->children.begin();
				for (;	siblings!=el->parent->children.end();
						siblings++)
				{
					if (*siblings!=el && (*siblings)->type==1)
					{
						return false;
					}
				}
				return true;
			}
			else if (ss->str1==std::string("only-of-type"))
			{
				if (el->parent==NULL) return false;
				std::vector<HTMLElement*>::iterator siblings = el->parent->children.begin();
				for (;	siblings!=el->parent->children.end();
						siblings++)
				{
					if (*siblings!=el && (*siblings)->type==1 && (*siblings)->text==el->text)
					{
						return false;
					}
				}
				return true;
			}
			else if (ss->str1==std::string("first-of-type"))
			{
				if (el->parent==NULL) return false;
				std::vector<HTMLElement*>::iterator siblings = el->parent->children.begin();
				for (;	siblings!=el->parent->children.end();
						siblings++)
				{
					if ((*siblings)->type==1 && (*siblings)->text==el->text)
					{
						return *siblings == el;
					}
				}
				std::cout << RED << "Fatal error: almwlkrcmwlkrmslkfm\n" << NOCLR;
			}
			else if (ss->str1==std::string("last-of-type"))
			{
				if (el->parent==NULL) return false;
				std::vector<HTMLElement*>::iterator siblings = el->parent->children.end();
				siblings--;
				for (;	siblings>=el->parent->children.begin();
						siblings--)
				{
					if ((*siblings)->type==1 && (*siblings)->text==el->text)
					{
						return el == *siblings;
					}
				}
				std::cout << RED << "Fatal error: klkmsdfmkasdfkwe\n" << NOCLR;
			}
			else
			{
				std::cout << RED << "Error: Unknown CSS selector :" << ss->str1 << "\n" << NOCLR;
			}
		}
		break;
		default:
		{
			std::cout << RED << "FATAL ERROR: applies() CALLED WITH A WRONG CASE (" <<  ss->type << ")!\n" << NOCLR;
		}
		break;
	}
	return false;
}
