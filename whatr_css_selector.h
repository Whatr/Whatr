#ifndef css_selector_included
#define css_selector_included yes

#include "whatr_html_yaccer.h"

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
void CSSSelectAll(HTMLElement* top, std::vector<HTMLElement*>* dest);
bool applies(CSSSubSelector* ss, HTMLElement* el);

#endif
