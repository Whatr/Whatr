class CSSToken
{
public:
	int type; // 0 = string, 1 = operator
	std::string text;
};
class cssLexArgs
{
public:
	int* lexingCSS;
	std::vector<CSSToken>* CSSTokens;
	std::string* inputCSS;
	cssLexArgs(int* lexingCSS,
			std::vector<CSSToken>* CSSTokens,
			std::string* inputCSS):
		lexingCSS(lexingCSS),
		CSSTokens(CSSTokens),
		inputCSS(inputCSS){};
};
void* cssLexThreadFunc(void* args);
