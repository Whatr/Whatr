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

#ifndef css_selector_included
#define css_selector_included yes

#include "html_yaccer.h"

class CSSSubSelector
{
public:
	std::string str1;
	std::string str2;
	int type;
	/*
	type	CSS		meaning								example
	-2				inside [] but haven't encountered operator
	-1				operator
	0		a		a elements							<A></A>
	1		.a		elements with a class				<P class="a"></P>
	2		[a]		elements with an a attribute		<P a="hello"></P>
	3		[a=b]	elements with a="b"					<P a="b"></p>
	4		[a~=b]	elements where a contains "b"		<P a="grass is blue"></P>
	5		[a|=b]	elements where a starts with "b"	<P a="blue grass exists"></P>
	6		[a^=b]	asdkfjalksjdflkasjdflkjasdlfkjas	<P
	7		:a		asdf
	8		:a(b)	
	9		:not(...)
	10		*
	11		[a$=b]
	12		[a*=b]
	*/
};
class CSSSelector
{
public:
	std::vector<CSSSubSelector> subSelectors;
};
std::vector<HTMLElement*> CSSSelect(HTMLElement* from, CSSSelector* selector);
std::vector<HTMLElement*> CSSSelect2(HTMLElement* from, CSSSubSelector ss, bool recurse);
std::vector<HTMLElement*> CSSSelectSiblings(HTMLElement* from, CSSSubSelector ss, bool directlyAfter);
void CSSSelectAll(HTMLElement* top, std::vector<HTMLElement*>* dest);
bool applies(CSSSubSelector* ss, HTMLElement* el);

#endif
