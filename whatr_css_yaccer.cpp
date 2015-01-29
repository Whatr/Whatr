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
#include "whatr_css_yaccer.h"

void* cssYaccThreadFunc(void* args)
{
	PRINT(cssYaccThreadFunc start);
	
	cssYaccArgs* c = (cssYaccArgs*)args;
	
	int* yaccingCSS = c->yaccingCSS;
	std::vector<CSSToken>* CSSTokens = c->CSSTokens;
	std::vector<CSSClass>* CSSClasses = c->CSSClasses;
	*yaccingCSS = 1;
	PRINT(cssYaccThreadFunc has set yaccingCss=1);
	std::cout << "Yaccing css:\n";
	
	////////////////////
	// Yacc CSS
	{
		int i = 0;
		
		int inWhat = 0; // 0 = in selector, 1 = in rule pre :, 2 = in rule post :
		
		
		//------------ This only applies when inWhat==0
		int inWhatWhat = 0; // 0 = normal, 1 = inside []
		CSSSubSelector curSS; // <-- for when inWhatWhat = 1;
		curSS.type = -2;
		//------------
		
		CSSSelector curS;
		CSSClass curC;
		
		while
		(
			i<CSSTokens->size()
		)
		{
			CSSToken t = CSSTokens->at(i);
			std::cout << "##############################################\n";
			std::cout << "Now at token " << t.type << ": " << t.text << "\n";
			std::cout << "inWhat="<<inWhat<<" inWhatWhat="<<inWhatWhat<<"\n";
			
			if (inWhat==0) // In selector curS
			{
				if (inWhatWhat==0)
				{
					if (t.type==0) // Current token is a string
					{
						std::cout << "Checkpoint\n";
						if (curS.subSelectors.size()>0 && curS.subSelectors.back().type!=-1) // Not an op
						{
							std::cout << "Encountered space operator\n";
							CSSSubSelector sub;
							sub.str1 = std::string(" ");
							sub.type = -1;
							curS.subSelectors.push_back(sub);
						}
						if (t.text.at(0)=='#')
						{
							std::cout << "Checkpoint 1\n";
							std::cout << "Encountered ID " << t.text << "\n";
							CSSSubSelector sub;
							sub.str1 = std::string("id");
							sub.str2 = t.text.substr(1);
							sub.type = 3;
							curS.subSelectors.push_back(sub);
						}
						else if (t.text.at(0)=='.')
						{
							std::cout << "Checkpoint 2\n";
							std::cout << "Encountered Class " << t.text << "\n";
							CSSSubSelector sub;
							sub.str1 = t.text.substr(1);
							sub.type = 1;
							curS.subSelectors.push_back(sub);
						}
						else
						{
							std::cout << "Checkpoint 3\n";
							std::cout << "Encountered tag name " << t.text << "\n";
							CSSSubSelector sub;
							sub.str1 = t.text;
							sub.type = 0;
							curS.subSelectors.push_back(sub);
						}
					}
					else if (t.type==1)
					{
						std::cout << "Encountered Operator " << t.text << "\n";
						if (t.text==std::string("{"))
						{
							std::cout << "Entered a class: Now in a rule before :\n";
							inWhat = 1;
							curC.selector = curS;
							curS = CSSSelector();
						}
						else if (t.text==std::string("["))
						{
							std::cout << "Entered a [...] structure\n";
							inWhatWhat = 1;
						}
						else
						{
							std::cout << "Added Operator " << t.text << " to selector\n";
							CSSSubSelector sub;
							sub.str1 = t.text;
							sub.type = -1;
							curS.subSelectors.push_back(sub);
						}
					}
					else
					{
						std::cout<<RED<<"CSS Yacc fatal error: t.type="<<t.type<<"\n"<<NOCLR;
					}
				}
				else if (inWhatWhat==1)
				{
					if (t.type==0) // Current token is a string
					{
						if (curSS.type==-2) // If the operator has not been seen yet
						{
							curSS.str1 = t.text;
						}
						else
						{
							curSS.str2 = t.text;
						}
					}
					else if (t.type==1) // Current token is an operator
					{
						if (t.text==std::string("]"))
						{
							if (curSS.type==-2) // If the operator has not been seen
							{
								// If the operator hasn't been seen yet, that means
								// that the type has not been set yet.
								curSS.type = 2;
							}
							curS.subSelectors.push_back(curSS);
							inWhatWhat = 0;
							curSS = CSSSubSelector();
							curSS.type = -2;
						}
						else if (t.text==std::string("="))
						{
							curSS.type = 3;
						}
						else if (t.text==std::string("~="))
						{
							curSS.type = 4;
						}
						else if (t.text==std::string("|="))
						{
							curSS.type = 5;
						}
						else if (t.text==std::string("^="))
						{
							curSS.type = 6;
						}
						else
						{
							std::cout << RED << "CSS Yacc error: Unexpected operator token " << t.text << "\n" << NOCLR;
						}
					}
					else
					{
						std::cout<<RED<<"CSS Yacc fatal error: t.type="<<t.type<<"\n"<<NOCLR;
					}
				}
				else
				{
						std::cout<<RED<<"CSS Yacc fatal error: inWhatWhat="<<inWhatWhat<<"\n"<<NOCLR;
				}
			}
			else if (inWhat==1) // in rule before :
			{
				if (t.type==0) // Current token is a string
				{
					if (curC.ruleNames.size()!=curC.ruleValues.size())
					{
						std::cout << RED << "CSS Yacc error: Unexpected string " << t.text << "\n" << NOCLR;
					}
					else
					{
						std::cout << "Added rule name " << t.text << "\n";
						curC.ruleNames.push_back(t.text);
					}
				}
				else if (t.type==1) // Current token is an op
				{
					if (t.text==std::string(":"))
					{
						std::cout << "Encountered : - now in rule after :\n";
						inWhat = 2;
					}
					else if (t.text==std::string("}"))
					{
						std::cout << "End of class found.\n";
						inWhat = 0;
						CSSClasses->push_back(curC);
						curC = CSSClass();
					}
					else
					{
						std::cout << RED << "CSS Yacc error: Unexpected operator " << t.text << NOCLR << "\n";
					}
				}
				else
				{
					std::cout << RED << "CSS Yacc error: t.type=" << t.type << "\n" << NOCLR;
				}
			}
			else if (inWhat==2) // in rule after :
			{
				if (t.type==0) // Current token is a string
				{
					if (curC.ruleNames.size()==curC.ruleValues.size())
					{
						std::cout << RED << "CSS Yacc error: Unexpected string " << t.text << "\n" << NOCLR;
					}
					else
					{
						std::cout << "Added rule value " << t.text << "\n";
						curC.ruleValues.push_back(t.text);
					}
				}
				else if (t.type==1) // Current token is an op
				{
					if (t.text==std::string(";"))
					{
						std::cout << "Encountered ; - now in rule before :\n";
						inWhat = 1;
					}
					else
					{
						std::cout << RED << "CSS Yacc error: Unexpected operator " << t.text << NOCLR << "\n";
					}
				}
				else
				{
					std::cout << RED << "CSS Yacc error: t.type=" << t.type << "\n" << NOCLR;
				}
			}
			else
			{
				std::cout << RED << "CSS fatal yacc Error: inWhat=" << inWhat << "!!!! THIS SHOULD NEVER HAPPEN! PLEASE CREATE A BUG REPORT ABOUT THIS!\n" << NOCLR;
			}
			std::cout << "#-------------------------------------------------\n";
			std::cout << "curS:\n";
			for (int i=0;i<curS.subSelectors.size();i++)
			{
				std::cout << curS.subSelectors.at(i).str1 << " " << curS.subSelectors.at(i).str2 << " " << curS.subSelectors.at(i).type << "\n";
			}
			i++;
		}
	}
	PRINT(cssYaccThreadFunc end);
	
	
	
	
	*yaccingCSS = 0;
	pthread_exit(NULL);
}
