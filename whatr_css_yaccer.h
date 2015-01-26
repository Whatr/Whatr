class CSSSubSelector
{
	std::string str1;
	std::string str2;
	int type;
	/*
	type	CSS		meaning								example
	0		[a]		elements with an a attribute		<P a="hello"></P>
	1		[a=b]	elements with a="b"					<P a="b"></p>
	2		[a~=b]	elements where a contains "b"		<P a="grass is blue"></P>
	3		[a|=b]	elements where a starts with "b"	<P a="blue grass exists"></P>
	4		[a^=b]	asdkfjalksjdflkasjdflkjasdlfkjas	<P
	5
	6
	7
	8
	*/
};
class CSSSelector
{
	std::vector<CSSSubSelector> subSelectors;
	std::vector<int> operators;
	/*
	operator	CSS		meaning						example (upper-case selected)
	0			a,b		union						<A></A><B></B>
	1			a b		all b that are inside an a	<a><c><B></B></c></a>
	2			a>b		all b whose parent is an a	<a><B></B></a>
	3			a+b		all b directly after an a	<a></a><B></B>
	4			a~b		all b after an a			<a></a><c></c><B></B>
	*/
};
class CSSClass
{
	
	std::vector<std::string> ruleProperties;
	std::vector<std::string> ruleValues;
};
