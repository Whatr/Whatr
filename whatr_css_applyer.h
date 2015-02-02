// https://www.youtube.com/watch?v=4uOxOgm5jQ4

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
