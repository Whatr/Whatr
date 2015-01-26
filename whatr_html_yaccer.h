#include <string>
#include <vector>

class HTMLElement
{
public:
	// Html yaccer stuff:
	int type; // 0 = text, 1 = opening
	std::string text; // if (type==0) { this is text } else { this is tag name }
	std::vector<std::string> argNames;
	std::vector<std::string> argValues;
	HTMLElement* parent;
	std::vector<HTMLElement*> children;
	
	// Css yaccer stuff:
	std::vector<std::string> styleFields;
	std::vector<std::string> styleValues;
	
	// Prerenderer stuff:
	int sizeRelativity = 0; // 0 = no relativity, 1 = 
	int width = 0;
	int height = 0;
};
class htmlYaccArgs
{
public:
	int* lexingPage;
	int* yaccingPage;
	std::vector<HTMLTag>* HTMLTags;
	std::vector<HTMLElement*>* HTMLElements;
	htmlYaccArgs(int* lexingPage,
			int* yaccingPage,
			std::vector<HTMLTag>* HTMLTags,
			std::vector<HTMLElement*>* HTMLElements):
		lexingPage(lexingPage),
		yaccingPage(yaccingPage),
		HTMLTags(HTMLTags),
		HTMLElements(HTMLElements){};
};
void* htmlYaccThreadFunc(void* args);

