#ifndef css_yaccer_included
#define css_yaccer_included yes

#include "whatr_css_lexer.h"
#include "whatr_css_selector.h"

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

#endif
