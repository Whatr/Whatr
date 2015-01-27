#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <cctype>

#include "whatr_log_funcs.hpp"
#include "whatr_css_lexer.h"

void* cssLexThreadFunc(void* args)
{
	PRINT(cssLexThreadFunc start);
	
	cssLexArgs* c = (cssLexArgs*)args;
	
	int* lexingCSS = c->lexingCSS;
	std::vector<CSSToken>* CSSTokens = c->CSSTokens;
	std::string* inputCSS = c->inputCSS;
	
	
	*lexingCSS = 1;
	PRINT(cssLexThreadFunc has set lexingCss=1);
	std::cout << "Lexing css:\n" << *inputCSS << "\n";
	
	////////////////////
	// Lex CSS
	{
		int i = 0;
		
		int currentType = -1; // 0 = string, 1 = operator
		
		std::string buffer = std::string("");
		
		while
		(
			i<inputCSS->length()
		)
		{
			char c = inputCSS->at(i);
			if (currentType==-1)
			{
				if (c==' ' || c=='\n' || c=='\t' || c=='\r')
				{
					// Ignore whitespaces when not inside anything
				}
				else if (	c==':' ||	// color: blue;
							c==';')
				{
					currentType = 1;
					buffer += c;
				}
				else if (c>='a' && c<='z')
				{
					currentType = 0;
					buffer += c;
				}
			}
			else if (currentType==0)
			{
				if (c==' ' || c=='\n' || c=='\t' || c=='\r')
				{
					CSSToken t;
					t.type = 0;
					t.text = buffer;
					CSSTokens->push_back(t);
					buffer = std::string("");
					
				}
			}
			i++;
		}
	}
	*lexingCSS = 0;
	PRINT(htmlLexThreadFunc end);
	pthread_exit(NULL);
}
