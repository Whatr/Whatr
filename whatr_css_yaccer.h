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
	7
	*/
};
class CSSSelector
{
public:
	std::vector<CSSSubSelector> subSelectors;
};
class CSSClass
{
public:
	CSSSelector selector;
	std::vector<std::string> ruleNames;
	std::vector<std::string> ruleValues;
};
class cssYaccArgs
{
public:
	int* yaccingCSS;
	std::vector<CSSToken>* CSSTokens;
	std::vector<CSSClass>* CSSClasses;
	cssYaccArgs(int* yaccingCSS,
				std::vector<CSSToken>* CSSTokens,
				std::vector<CSSClass>* CSSClasses):
		yaccingCSS(yaccingCSS),
		CSSTokens(CSSTokens),
		CSSClasses(CSSClasses){};
};
void* cssYaccThreadFunc(void* args);
