// https://www.youtube.com/watch?v=4uOxOgm5jQ4

#ifndef css_applyer_included
#define css_applyer_included yes

#include "whatr_html_yaccer.h"

class cssApplyArgs
{
public:
	int* applyingCSS;
	std::vector<CSSClass>* CSSClasses;
	HTMLElement* destination;
	cssApplyArgs(int* applyingCSS,
				std::vector<CSSClass>* CSSClasses,
				HTMLElement* destination):
		applyingCSS(applyingCSS),
		CSSClasses(CSSClasses),
		destination(destination){};
};
void* cssApplyThreadFunc(void* args);

#endif
