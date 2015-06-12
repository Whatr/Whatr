/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

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
#include "const_str.h"

void* cssLexThreadFunc(void* args)
{
	PRINT(cssLexThreadFunc start);
	
	cssLexArgs* c = (cssLexArgs*)args;
	
	int* lexingCSS = c->lexingCSS;
	std::vector<CSSToken>* CSSTokens = c->CSSTokens;
	ConstStr inputCSS = c->inputCSS;
	
	
	*lexingCSS = 1;
	PRINT(cssLexThreadFunc has set lexingCss=1);
	std::cout << "Lexing css:\n";
	inputCSS.printLine();
	
	////////////////////
	// Lex CSS
	{
		ConstStrIterator i = inputCSS.iterate();
		
		CSSTokenType currentType = TOKEN_TYPE_NOTHING;
		int seenLetter = 0;
		int seenDigit = 0;
		
		//std::string buffer = std::string("");
		int bufferStart = -1;
		
		int inComment = 0;
		
		char inQuotes = 0;
		bool backslashing = false;
		int inClass = 0;
		
		while
		(
			i.pos<inputCSS.length
		)
		{
			char c = *i;
			//std::cout << "Now at char " << c << "\n";
			
			//printf("status=[cT%i, sL%i, sD%i, bS%i, c%i, iC%i]\n", currentType, seenLetter, seenDigit, bufferStart, count, inComment);
			
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
			else if (c=='*' && i>0 && inputCSS[i.pos-1]=='/')
			{
				std::cout << GREEN << "CSS lexer notice: Starting comment...\n" << NOCLR;
				inComment = 1;
			}
			else if (inQuotes)
			{
				if (backslashing) backslashing = false;
				else if (c=='\\') backslashing = true;
				else if (c==inQuotes)
				{
					CSSToken t;
					t.type = inQuotes=='"' ? TOKEN_TYPE_STRING_DOUBLE_QUOTES : TOKEN_TYPE_STRING_SINGLE_QUOTES;
					t.text = inputCSS.subString(bufferStart, i.pos-bufferStart);
					CSSTokens->push_back(t);
					inQuotes = 0;
				}
			}
			else if (c=='\'' || c=='"')
			{
				if (currentType!=TOKEN_TYPE_NOTHING)
				{
					CSSToken t;
					t.type = currentType;
					t.text = inputCSS.subString(bufferStart, i.pos-bufferStart);
					CSSTokens->push_back(t);
					currentType = TOKEN_TYPE_NOTHING;
				}
				bufferStart = i.pos+1;
				inQuotes = c;
			}
			else if (currentType==TOKEN_TYPE_NOTHING)
			{
				if (c=='!')
				{
					if (inputCSS.subString(i.pos+1, 9)==std::string("important"))
					{ // TODO support !important
						std::cout << RED << "Warning: !important is not supported yet. ignoring it.\n" << NOCLR;
						i += 8;
						std::cout << "==" << *i << "\n";
						i++;
						std::cout << "==" << *i << "\n";
						i++;
						std::cout << "==" << *i << "\n";
						continue;
					}
				}
				else if (c=='@')
				{
					if (inputCSS.subString(i.pos+1, 5)==std::string("media"))
					{ // TODO support @media
						std::cout << RED << "Warning: @media is not supported yet. ignoring it, and everything it contains\n" << NOCLR;
						int brutoBracketCount = 0;
						int nettoBracketCount = 0;
						while (brutoBracketCount==0 || nettoBracketCount>0)
						{
							i++;
							if (*i == '{')
								brutoBracketCount++, nettoBracketCount++;
							else if (*i == '}')
								brutoBracketCount++, nettoBracketCount--;
							//std::cout << *i << brutoBracketCount << nettoBracketCount << "\n";
						}
						i++;
						continue;
					}
				}
				else if (c==' ' || c=='\n' || c=='\t' || c=='\r')
				{
					if (CSSTokens->size()>0)
					{
						CSSToken tt = CSSTokens->at(CSSTokens->size()-1);
						if (tt.type!=TOKEN_TYPE_OPERATOR || tt.text!=std::string(" "))
						{
							if (!inClass)
							{
								CSSToken t;
								t.type = TOKEN_TYPE_OPERATOR;
								t.text = std::string(" ");
								CSSTokens->push_back(t);
							}
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
					t.type = TOKEN_TYPE_OPERATOR;
					t.text = inputCSS.subString(i.pos, 1);
					CSSTokens->push_back(t);
					if (c=='{') inClass = 1;
					else if (c=='}') inClass = 0;
				}
				else if (
					c=='~' || // CSS op chars that may or may not be the start of a multichar op
					c=='*' ||
					c==':' ||
					c=='|' || // CSS op chars that are always the start of a multichar op
					c=='^' ||
					c=='$')
				{
					currentType = TOKEN_TYPE_OPERATOR;
					bufferStart = i.pos;
					//buffer += c;
				}
				else if (	(c>='a' && c<='z') ||
							(c>='A' && c<='Z') ||
							(c>='0' && c<='9') ||
							c=='#' ||
							c=='-' ||
							c=='.'
						)
				{
					//std::cout << "strnq start" << c << "\n";
					currentType = TOKEN_TYPE_STRING_NO_QUOTES;
					bufferStart = i.pos;
					//buffer += c;
					continue;
				}
				else if (c=='/'){} // Ignore /
				else
				{
					std::cout << RED << "CSS Lexer error: Unexpected character '" << c << "' (in " << currentType << ")\n" << NOCLR;
				}
			}
			else if (currentType==TOKEN_TYPE_STRING_NO_QUOTES)
			{
				if (c>='0' && c<='9')
				{
					//buffer += c;
					seenDigit = 1;
				}
				else if (((c>='a' && c<='z') ||
						  (c>='A' && c<='Z') ||
						  (c=='#'))
						&& (seenLetter || !seenDigit))
				{
					//buffer += c;
					seenLetter = 1;
				}
				else if ((c=='-' && !seenDigit) ||
						 (c=='.' && (bufferStart>=i.pos || seenDigit)))
				{
					//buffer += c;
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
						t.type = TOKEN_TYPE_PERCENTAGE;
						t.text = inputCSS.subString(bufferStart, i.pos-bufferStart);
						CSSTokens->push_back(t);
						bufferStart = i.pos+1;
						currentType = TOKEN_TYPE_NOTHING;
						seenLetter = seenDigit = 0;
					}
				}
				else
				{
					if (seenLetter) // It was a string
					{
						//std::cout << c << "elsey 1\n";
						CSSToken t;
						t.type = TOKEN_TYPE_STRING_NO_QUOTES;
						t.text = inputCSS.subString(bufferStart, i.pos-bufferStart);
						CSSTokens->push_back(t);
						bufferStart = i.pos+1;
						currentType = TOKEN_TYPE_NOTHING;
						seenLetter = seenDigit = 0;
					}
					else if (seenDigit) // It was a number
					{
						//std::cout << c << "elsey 2\n";
						CSSToken t;
						t.type = TOKEN_TYPE_NUMBER;
						t.text = inputCSS.subString(bufferStart, i.pos-bufferStart);
						CSSTokens->push_back(t);
						bufferStart = i.pos+1;
						currentType = TOKEN_TYPE_NOTHING;
						seenLetter = seenDigit = 0;
					}
					else
					{
						//std::cout << RED << "Lexer error: Lol wtf\n" << NOCLR;
					}
					if (c==' ' || c=='\n' || c=='\t' || c=='\r')
					{
						if (!inClass)
						{
							CSSToken t;
							t.type = TOKEN_TYPE_OPERATOR;
							t.text = std::string(" ");
							CSSTokens->push_back(t);
						}
					}
					else
					{
						continue; // Rerun the loop for the current character (operator) by not incrementing i
					}
				}
			}
			else if (currentType==TOKEN_TYPE_OPERATOR)
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
					c==':' ||
					c=='#'
					))
				{
					CSSToken t;
					t.type = TOKEN_TYPE_OPERATOR;
					t.text = inputCSS.subString(bufferStart, i.pos-bufferStart);
					CSSTokens->push_back(t);
					bufferStart = i.pos;
					currentType = TOKEN_TYPE_NOTHING;
					seenLetter = seenDigit = 0;
					continue;
				}
				else if (c==' ' || c=='\n' || c=='\t' || c=='\r')
				{
					CSSToken t;
					t.type = TOKEN_TYPE_OPERATOR;
					t.text = inputCSS.subString(bufferStart, i.pos-bufferStart);
					CSSTokens->push_back(t);
					bufferStart = i.pos+1;
					currentType = TOKEN_TYPE_NOTHING;
					seenLetter = seenDigit = 0;
					if (!inClass)
					{
						CSSToken t;
						t.type = TOKEN_TYPE_OPERATOR;
						t.text = std::string(" ");
						CSSTokens->push_back(t);
					}
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
