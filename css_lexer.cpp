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
#include "css_lexer.h"

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
		int seenLetter = 0;
		int seenDigit = 0;
		
		std::string buffer = std::string("");
		
		int count = 0;
		
		int inComment = 0;
		
		while
		(
			i<inputCSS->length() && count < 10000
		)
		{
			count++;
			char c = inputCSS->at(i);
			//std::cout << "Now at char " << c << "\n";
			
			if (inComment==2)
			{
				if (c=='/')
				{
					std::cout << GREEN << "CSS lexer notice: Ending comment...\n" << NOCLR;
					inComment = 0;
				}
				else
				{
					inComment = 1;
				}
			}
			else if (inComment==1 && c=='*')
			{
				inComment++;
			}
			else if (inComment){}
			else if (c=='*' && i>0 && inputCSS->at(i-1)=='/')
			{
				std::cout << GREEN << "CSS lexer notice: Starting comment...\n" << NOCLR;
				inComment = 1;
			}
			else if (currentType==-1)
			{
				if (c==' ' || c=='\n' || c=='\t' || c=='\r')
				{
					if (CSSTokens->size()>0)
					{
						CSSToken tt = CSSTokens->at(CSSTokens->size()-1);
						if (tt.type!=1 || tt.text!=std::string(" "))
						{
							CSSToken t;
							t.type = 1;
							t.text = std::string(" ");
							CSSTokens->push_back(t);
						}
					}
				}
				else if (	c==';' || // CSS op chars that always stand alone...
							c=='{' ||
							c=='}' ||
							c=='(' ||
							c==')' ||
							c=='[' ||
							c==']' ||
							c==',' ||
							c=='>' ||
							c=='+' ||
							c=='=')		// ...or never appear as the first char of a multichar op
				{
					CSSToken t;
					t.type = 1;
					t.text = std::string("")+c;
					CSSTokens->push_back(t);
				}
				else if (
					c=='~' || // CSS op chars that may or may not be the start of a multichar op
					c=='*' ||
					c==':' ||
					c=='|' || // CSS op chars that are always the start of a multichar op
					c=='^' ||
					c=='$')
				{
					currentType = 1;
					buffer += c;
				}
				else if (	(c>='a' && c<='z') ||
							(c>='0' && c<='9') ||
							c=='#' ||
							c=='-' ||
							c=='.'
						)
				{
					currentType = 0;
					buffer += c;
				}
				else if (c=='/'){} // Ignore /
				else
				{
					std::cout << RED << "CSS Lexer error: Unexpected character '" << c << "' (in " << currentType << ")\n" << NOCLR;
				}
			}
			else if (currentType==0)
			{
				if (c>='0' && c<='9')
				{
					buffer += c;
					seenDigit = 1;
				}
				else if (((c>='a' && c<='z') ||
						  (c>='A' && c<='Z'))
						&& (seenLetter || !seenDigit))
				{
					buffer += c;
					seenLetter = 1;
				}
				else if ((c=='-' && !seenDigit) ||
						 (c=='.' && (buffer.size()==0 || seenDigit)))
				{
					buffer += c;
				}
				else if (c=='/') {} // Ignore /
				else if (c=='%')
				{
					if (seenLetter || !seenDigit)
					{
						std::cout << RED << "CSS Lexer error: Unexpected character '" << c << "'\n" << NOCLR;
					
					}
					else // It was a percentage
					{
						CSSToken t;
						t.type = 2;
						t.text = buffer;
						CSSTokens->push_back(t);
						buffer = std::string("");
						currentType = -1;
						seenLetter = seenDigit = 0;
					}
				}
				else
				{
					if (seenLetter) // It was a string
					{
						CSSToken t;
						t.type = 0;
						t.text = buffer;
						CSSTokens->push_back(t);
						buffer = std::string("");
						currentType = -1;
						seenLetter = seenDigit = 0;
					}
					else if (seenDigit) // It was a number
					{
						CSSToken t;
						t.type = 3;
						t.text = buffer;
						CSSTokens->push_back(t);
						buffer = std::string("");
						currentType = -1;
						seenLetter = seenDigit = 0;
					}
					else
					{
						std::cout << RED << "Lexer error: Lol wtf\n";
					}
					if (c==' ' || c=='\n' || c=='\t' || c=='\r')
					{
						CSSToken t;
						t.type = 1;
						t.text = std::string(" ");
						CSSTokens->push_back(t);
					}
					else
					{
						continue; // Rerun the loop for the current character (operator) by not incrementing i
					}
				}
			}
			else if (currentType==1)
			{
				if (c=='/'){} // Ignore /
				else if ((c>='a' && c<='z') ||
					(c>='A' && c<='Z') ||
					(c>='0' && c<='9') ||
					 c=='-' ||
			 		(c==';' ||
					c=='.' ||
					c=='{' ||
					c=='}' ||
					c=='(' ||
					c==')' ||
					c=='[' ||
					c==']' ||
					c==',' ||
					c=='>' ||
					c=='+' ||
					c==':'
					))
				{
					CSSToken t;
					t.type = 1;
					t.text = buffer;
					CSSTokens->push_back(t);
					buffer = std::string("");
					currentType = -1;
					seenLetter = seenDigit = 0;
					continue;
				}
				else if (c==' ' || c=='\n' || c=='\t' || c=='\r')
				{
					CSSToken t;
					t.type = 1;
					t.text = buffer;
					CSSTokens->push_back(t);
					buffer = std::string("");
					currentType = -1;
					seenLetter = seenDigit = 0;
					{
						CSSToken t;
						t.type = 1;
						t.text = std::string(" ");
						CSSTokens->push_back(t);
					}
				}
				else
				{
					buffer += c;
				}
			}
			else
			{
				std::cout << RED << "CSS Lexer error: Fatal error: Invalid currentType of " << currentType << "! This should never happen! Please contact the developer(s)!\n" << NOCLR;
			}
			i++;
		}
	}
	PRINT(cssLexThreadFunc end);
	*lexingCSS = 0;
	pthread_exit(NULL);
}
