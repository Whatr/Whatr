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
		if (ss->type == -1)
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
		else if (ss->type == 10) // *
		{
			/*
			Select all elements. Every single one.
			*/
			selected = std::vector<HTMLElement*>();
			CSSSelectAll(from, &selected);
			op = '\0';
		}
		else if (op==' ') // (ss->type == 0)
		{
			/*
			Delete current selection and make a new one based upon it
			*/
			if (op==' ' || op=='>') // Select children
			{
				std::cout << "op==' ' || op=='>'\n";
				std::vector<HTMLElement*> newVector;
				for (	std::vector<HTMLElement*>::iterator els=selected.begin();
						els!=selected.end();
						els++)
				{
					std::vector<HTMLElement*> addThis = CSSSelect2(*els, *ss, op==' ');
					//std::cout << "addThis.size()==" << addThis.size() << "\n";
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
					//std::cout << "addThis.size()==" << addThis.size() << "\n";
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
				if (op!='\0') std::cout << RED << "Fatal CSS selector error: inside unknown op='" << op << "'\n" << NOCLR;
				else std::cout << RED << "Fatal CSS selector error: inside zero op!\n" << NOCLR; // TODO handle zero op correctly
			}
			op = '\0';
		}
		else if (	op=='\0'
					/*(ss->type >= 2 && ss->type <= 6) ||
					ss->type == 11 ||
					ss->type==12*/
				)
		{
			/*
			Use ss to select elements from the current selection that match it
			*/
			//std::cout << "case 2|3|4|5|6|11|12\nop = " << ((int)op) << "\n";
			std::vector<HTMLElement*> newVector;
			for (	std::vector<HTMLElement*>::iterator els=selected.begin();
					els!=selected.end();
					els++)
			{
				std::cout << "loop\n";
				if (applies(&*ss, *els)) newVector.push_back(*els);
			}
			selected = newVector;
			//std::cout << "break case 2|3|4|5|6|11|12\n";
		}
		else
		{
				std::cout << RED << "TODO: ss type=" << ss->type << " str1=" << ss->str1 << " op=" << ((int)op) << "\n" << NOCLR;
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
	//std::cout << "from=<" << from->text << ">\n";
	std::vector<HTMLElement*> ret;
	bool started = false;
	for (	std::vector<HTMLElement*>::iterator els=from->parent->children.begin();
						els!=from->parent->children.end();
						els++)
	{
		if ((*els)->type==1)
		{
			//std::cout << "@ child <" << (*els)->text << ">\n";
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
			for (	std::vector<std::string>::iterator args=el->argNames.begin();
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
			std::vector<std::string>::iterator args = el->argNames .begin();
			std::vector<std::string>::iterator vals = el->argValues.begin();
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
			std::vector<std::string>::iterator args = el->argNames .begin();
			std::vector<std::string>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1)
				{
					size_t foundLoc = vals->find(ss->str2);
					if (foundLoc == std::string::npos)
					{
						// arg="a b c"
						// checking for "qwertyuiop"
						return false;
					}
					if (vals->length() == ss->str2.length())
					{
						// arg="a"
						// checking for "a"
						return true;
					}
					if (foundLoc==0)
					{
						// arg="a b c"
						// checking for "a"
						return vals->at(ss->str2.length())==' ';
					}
					if (
							vals->at(foundLoc-1)==' '
							&&
							(
								foundLoc+ss->str2.length()==vals->length()
								||
								vals->at(foundLoc+ss->str2.length())==' '
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
			std::vector<std::string>::iterator args = el->argNames .begin();
			std::vector<std::string>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1)
				{
					if (vals->length() < ss->str2.length()) return false;
					return
						(
							vals->substr(0, ss->str2.length()) == ss->str2
							&&
							(
								vals->length() == ss->str2.length()
								||
								vals->at(ss->str2.length()) == '-'
							)
						);
				}
			}
			return false;
		}
		break;
		case 6:		// ^=		elements having a str1 attribute and where str1 starts with str2
		{
			std::vector<std::string>::iterator args = el->argNames .begin();
			std::vector<std::string>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1)
				{
					if (vals->length() < ss->str2.length()) return false;
					return vals->substr(0, ss->str2.length()) == ss->str2;
				}
			}
			return false;
		}
		break;
		case 11:	// $=		elements having a str1 attribute and where str1 ends with str2
		{
			std::vector<std::string>::iterator args = el->argNames .begin();
			std::vector<std::string>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1)
				{
					if (vals->length() < ss->str2.length()) return false;
					return vals->substr(vals->length() - ss->str2.length(), ss->str2.length()) == ss->str2;
				}
			}
			return false;
		}
		break;
		case 12:	// *=		elements having a str1 attribute and where str1 contains str2
		{
			std::vector<std::string>::iterator args = el->argNames .begin();
			std::vector<std::string>::iterator vals = el->argValues.begin();
			for (;	args!=el->argNames.end();
					args++, vals++)
			{
				if (*args == ss->str1)
				{
					return vals->find(ss->str2) != std::string::npos;
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
