/* Copyright (C) 2015 Jesse Busman
/* This program is free software: you can redistribute it and/or modify
/* it under the terms of the GNU General Public License as published by
/* the Free Software Foundation, either version 3 of the License, or
/* (at your option) any later version.
/* This program is distributed in the hope that it will be useful,
/* but WITHOUT ANY WARRANTY; without even the implied warranty of
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/* GNU General Public License for more details.
/* You should have received a copy of the GNU General Public License
/* along with this program.  If not, see <http://www.gnu.org/licenses/>  */

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <cctype>

#include "log_funcs.hpp"
#include "css_yaccer.h"
#include "css_lexer.h"
#include "css_values.h"

CSSValue parseRuleValue(std::vector<CSSToken>* tokens, int start, int end)
{
	// start points to the :
	// end points to the token before the ;
	printf("parseRuleValue(..., %i, %i) = (..., %i-%s, %i-%s)\n", start, end, tokens->at(start).type, tokens->at(start).text.c_str(), tokens->at(end).type, tokens->at(end).text.c_str());
	int length = end-start;
	CSSValue ret;
	ret.text = 0;
	ret.length = 0;
	ret.color = 0;
	ret.time = 0;
	if (length<0)
	{
		ERROR(CSS error: WTF length of value < 0 wtf impossibru);
		return ret;
	}
	else if (length==0)
	{
		ERROR(Fatal CSS error: empty rule value);
		return ret;
	}
	else if (length==1) // 20  red  #FFF  #test
	{
		if (tokens->at(start+1).text==std::string("red"))
		{
			ret.color = 1;
			ret.colorValue = 0xFF000000;
		}
		else if (tokens->at(start+1).text==std::string("green"))
		{
			ret.color = 1;
			ret.colorValue = 0x00FF0000;
		}
		else if (tokens->at(start+1).text==std::string("blue"))
		{
			ret.color = 1;
			ret.colorValue = 0x0000FF00;
		}
		else
		{
			ret.length = 3;
			std::string::size_type sz;
			ret.lengthValue = std::stod(tokens->at(start+1).text, &sz);
		}
	}  
	else if (length==2) // 20px
	{
		if (tokens->at(start+2).text==std::string("px"))
		{
			ret.length = 1;
			std::string::size_type sz;
			ret.lengthValue = std::stod(tokens->at(start+1).text, &sz);
		}
		else if (tokens->at(start+2).text==std::string("em"))
		{
			ret.length = 2;
			std::string::size_type sz;
			ret.lengthValue = std::stod(tokens->at(start+1).text, &sz);
		}
		else if (tokens->at(start+2).text==std::string("ms"))
		{
			ret.time = 1;
			std::string::size_type sz;
			ret.timeValue = std::stod(tokens->at(start+1).text, &sz);
		}
		else if (tokens->at(start+2).text==std::string("s"))
		{
			ret.time = 2;
			std::string::size_type sz;
			ret.timeValue = std::stod(tokens->at(start+1).text, &sz);
		}
		else
		{
			std::cout << RED << "CSS Yaccer error: Unexpected something xD\n" << NOCLR;
		}
	}
	else
	{
		std::cout << RED << "CSS Yaccer error: Unexpected something xD\n" << NOCLR;
	}
		
	/*if (t2.type==0) // String
	{

	}
	else if (t2.type==1) // Operator
	{
			std::cout << RED << "CSS Yaccer error: Unexpected operator " << t2.text << "\n" << NOCLR;
	}
	else if (t2.type==2) // Percentage
	{

	}
	else
	{
		std::cout << RED << "CSS Yaccer error: Unexpected t2.type " << t2.type << "\n" << NOCLR;
	}*/
	return ret;
}

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
		int ruleValueStartI = -1;
		
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
			std::cout << "Now at token[" << i << "] "<< t.type << ": " << t.text << "\n";
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
							/*std::cout << "Encountered space operator\n";
							CSSSubSelector sub;
							sub.str1 = std::string(" ");
							sub.type = -1;
							curS.subSelectors.push_back(sub);*/
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
						if (t.text==std::string("*"))
						{
							std::cout << "Added Operator * to selector\n";
							CSSSubSelector sub;
							sub.str1 = t.text;
							sub.type = 10;
							curS.subSelectors.push_back(sub);
						}
						else if (t.text==std::string("{"))
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
						else if (t.text==std::string(":"))
						{
							std::cout << "Entered a possible :(...) structure\n";
							inWhatWhat = 2;
						}
						else
						{
							if (t.text==std::string(" ") && curS.subSelectors.size()==0)
							{
								std::cout << "Ignored whitespacing operator at start of selector\n";
							}
							else if (	curS.subSelectors.size()!=0 &&
										curS.subSelectors.back().type==-1 &&
										curS.subSelectors.back().str1==std::string(" ")
									)
							{
								std::cout << "Replaced whitespace operator in selector with " << t.text << "\n";
								curS.subSelectors.back().str1 = t.text;
							}
							else if (	curS.subSelectors.size()!=0 &&
										curS.subSelectors.back().type==-1 &&
										t.text==std::string(" "))
							{
								std::cout << "Ignored whitespace operator in selector\n";
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
						else if (t.text==std::string("$="))
						{
							curSS.type = 11;
						}
						else if (t.text==std::string("*="))
						{
							curSS.type = 12;
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
				else if (inWhatWhat==2)
				{
					if (t.type==0) // Current token is a string
					{
						if (curSS.type==8) // Inside a :lang(...) structure
						{
							curSS.str2 = t.text;
						}
						else
						{
							if (t.text==std::string("active") ||
								t.text==std::string("checked") ||
								t.text==std::string("disabled") ||
								t.text==std::string("empty") ||
								t.text==std::string("enabled") ||
								t.text==std::string("first-child") ||
								t.text==std::string("first-of-type") ||
								t.text==std::string("focus") ||
								t.text==std::string("hover") ||
								t.text==std::string("in-range") ||
								t.text==std::string("invalid") ||

								t.text==std::string("last-child") ||
								t.text==std::string("last-of-type") ||
								t.text==std::string("link") ||

								t.text==std::string("only-of-type") ||
								t.text==std::string("only-child") ||
								t.text==std::string("optional") ||
								t.text==std::string("out-of-range") ||
								t.text==std::string("read-only") ||
								t.text==std::string("read-write") ||
								t.text==std::string("required") ||
								t.text==std::string("root") ||
								t.text==std::string("target") ||
								t.text==std::string("valid") ||
								t.text==std::string("visited"))
							{
								curSS.type = 7;
								curSS.str1 = t.text;
								curS.subSelectors.push_back(curSS);
								curSS = CSSSubSelector();
								curSS.type = -2;
								inWhatWhat = 0;
							}
							else if (t.text==std::string("lang") ||
									t.text==std::string("nth-child") ||
									t.text==std::string("nth-last-child") ||
									t.text==std::string("nth-last-of-type") ||
									t.text==std::string("nth-of-type"))
							{
								curSS.type = 8;
								curSS.str1 = t.text;
							}
							else if (t.text==std::string("not"))
							{
								std::cout << RED << ":not is :not supported yet :(\n" << NOCLR;
							}
							else
							{
								std::cout << RED << "CSS Yacc error: Invalid CSS selector :" << t.text << "\n" << NOCLR;
							}
						}
					}
					else if (t.type==1) // The current token is an operator
					{
						if (t.text==std::string("("))
						{
							std::cout << "TODO9023845\n";
						}
						else if (t.text==std::string(")"))
						{
							curS.subSelectors.push_back(curSS);
							curSS = CSSSubSelector();
							curSS.type = -2;
							inWhatWhat = 0;
						}
						else
						{
							std::cout<<RED<<"CSS Yacc error: Unexpected operator "<<t.text<<"\n"<<NOCLR;
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
						ruleValueStartI = i;
					}
					else if (t.text==std::string("}"))
					{
						std::cout << "End of class found.\n";
						inWhat = 0;
						CSSClasses->push_back(curC);
						curC = CSSClass();
					}
					else if (t.text==std::string(" "))
					{
						// Ignore whitespaces
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
					/*if (curC.ruleNames.size()==curC.ruleValues.size())
					{
						std::cout << RED << "CSS Yacc error: Unexpected string " << t.text << "\n" << NOCLR;
					}
					else
					{
						std::cout << "Test :)";
					}*/
				}
				else if (t.type==1) // Current token is an op
				{
					if (t.text==std::string(";"))
					{
						std::cout << "Encountered ; - now in rule before :\n";
						inWhat = 1;
						curC.ruleValues.push_back(parseRuleValue(CSSTokens, ruleValueStartI, i-1));
					}
					else if (t.text==std::string("}"))
					{
						std::cout << RED << "CSS Yacc error: Unexpected end of class }. Assuming you forgot a ;\n" << NOCLR;
						inWhat = 0;
						CSSClasses->push_back(curC);
						curC = CSSClass();
					}
					else if (t.text==std::string(" "))
					{
						// Ignore whitespaces
					}
					else
					{
						std::cout << RED << "CSS Yacc error: Unexpected operator " << t.text << NOCLR << "\n";
					}
				}
				else
				{
					//std::cout << RED << "CSS Yacc error: t.type=" << t.type << "\n" << NOCLR;
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
