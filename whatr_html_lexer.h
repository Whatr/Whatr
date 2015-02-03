#ifndef html_lexer_included
#define html_lexer_included yes

class HTMLTag
{
public:
	int type; // 0 = text, 1 = opening, 2 = opening & self-closing, 3 = closing
	std::string text; // if (type==0) { this is text } else { this is tag name }
	std::vector<std::string> argNames;
	std::vector<std::string> argValues;
};
class htmlLexArgs
{
public:
	int* lexingPage;
	int* downloadingPage;
	std::vector<HTMLTag>* HTMLTags;
	std::vector<std::string>* headerFields;
	std::vector<std::string>* headerValues;
	std::string* downloadedHeaders;
	std::string* downloadedHTML;
	htmlLexArgs(int* adownloadingPage,
			int* alexingPage,
			std::vector<HTMLTag>* HTMLTags,
			std::vector<std::string>* headerFields,
			std::vector<std::string>* headerValues,
			std::string* downloadedHeaders,
			std::string* downloadedHTML):
		downloadingPage(adownloadingPage),
		lexingPage(alexingPage),
		HTMLTags(HTMLTags),
		headerFields(headerFields),
		headerValues(headerValues),
		downloadedHeaders(downloadedHeaders),
		downloadedHTML(downloadedHTML){};
};
void* htmlLexThreadFunc(void* args);

#endif
