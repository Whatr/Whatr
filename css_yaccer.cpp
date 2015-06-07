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
#include "css_properties.h"

CSSProperty parseRuleName(ConstStr name)
{
	if (name-=std::string("color")) return COLOR;
	if (name-=std::string("opacity")) return OPACITY;
	if (name-=std::string("background")) return BACKGROUND;
	if (name-=std::string("background-attachment")) return BACKGROUND_ATTACHMENT;
	if (name-=std::string("background-color")) return BACKGROUND_COLOR;
	if (name-=std::string("background-image")) return BACKGROUND_IMAGE;
	if (name-=std::string("background-position")) return BACKGROUND_POSITION;
	if (name-=std::string("background-repeat")) return BACKGROUND_REPEAT;
	if (name-=std::string("border")) return BORDER;
	if (name-=std::string("border-color")) return BORDER_COLOR;
	if (name-=std::string("border-style")) return BORDER_STYLE;
	if (name-=std::string("border-width")) return BORDER_WIDTH;
	if (name-=std::string("border-top")) return BORDER_TOP;
	if (name-=std::string("border-left")) return BORDER_LEFT;
	if (name-=std::string("border-right")) return BORDER_RIGHT;
	if (name-=std::string("border-bottom")) return BORDER_BOTTOM;
	if (name-=std::string("border-bottom-color")) return BORDER_BOTTOM_COLOR;
	if (name-=std::string("border-bottom-style")) return BORDER_BOTTOM_STYLE;
	if (name-=std::string("border-bottom-width")) return BORDER_BOTTOM_WIDTH;
	if (name-=std::string("border-top-color")) return BORDER_TOP_COLOR;
	if (name-=std::string("border-top-style")) return BORDER_TOP_STYLE;
	if (name-=std::string("border-top-width")) return BORDER_TOP_WIDTH;
	if (name-=std::string("border-left-color")) return BORDER_LEFT_COLOR;
	if (name-=std::string("border-left-style")) return BORDER_LEFT_STYLE;
	if (name-=std::string("border-left-width")) return BORDER_LEFT_WIDTH;
	if (name-=std::string("border-right-color")) return BORDER_RIGHT_COLOR;
	if (name-=std::string("border-right-style")) return BORDER_RIGHT_STYLE;
	if (name-=std::string("border-right-width")) return BORDER_RIGHT_WIDTH;
	if (name-=std::string("clear")) return CLEAR;
	if (name-=std::string("display")) return DISPLAY;
	if (name-=std::string("float")) return FLOAT;
	if (name-=std::string("height")) return HEIGHT;
	if (name-=std::string("width")) return WIDTH;
	if (name-=std::string("padding")) return PADDING;
	if (name-=std::string("padding-bottom")) return PADDING_BOTTOM;
	if (name-=std::string("padding-left")) return PADDING_LEFT;
	if (name-=std::string("padding-right")) return PADDING_RIGHT;
	if (name-=std::string("padding-top")) return PADDING_TOP;
	if (name-=std::string("vertical-align")) return VERTICAL_ALIGN;
	if (name-=std::string("margin")) return MARGIN;
	if (name-=std::string("margin-bottom")) return MARGIN_BOTTOM;
	if (name-=std::string("margin-top")) return MARGIN_TOP;
	if (name-=std::string("margin-left")) return MARGIN_LEFT;
	if (name-=std::string("margin-right")) return MARGIN_RIGHT;
	if (name-=std::string("letter-spacing")) return LETTER_SPACING;
	if (name-=std::string("line-height")) return LINE_HEIGHT;
	if (name-=std::string("text-align")) return TEXT_ALIGN;
	if (name-=std::string("text-transform")) return TEXT_TRANSFORM;
	if (name-=std::string("text-indent")) return TEXT_INDENT;
	if (name-=std::string("white-space")) return WHITE_SPACE;
	if (name-=std::string("word-spacing")) return WORD_SPACING;
	if (name-=std::string("text-decoration")) return TEXT_DECORATION;
	if (name-=std::string("font")) return FONT;
	if (name-=std::string("font-family")) return FONT_FAMILY;
	if (name-=std::string("font-size")) return FONT_SIZE;
	if (name-=std::string("font-style")) return FONT_STYLE;
	if (name-=std::string("font-variant")) return FONT_VARIANT;
	if (name-=std::string("font-weight")) return FONT_WEIGHT;
	if (name-=std::string("list-style")) return LIST_STYLE;
	if (name-=std::string("list-style-image")) return LIST_STYLE_IMAGE;
	if (name-=std::string("list-style-position")) return LIST_STYLE_POSITION;
	if (name-=std::string("list-style-type")) return LIST_STYLE_TYPE;
	return NONE;
}

std::vector<CSSValue>* parseRuleValue(std::vector<CSSToken>* tokens, int start, int end)
{
	// start points to the :
	// end points to the token before the ;
	
	std::vector<CSSValue>* CSSValues = new std::vector<CSSValue>;
	
	int length = end-start;
	if (length<0)
	{
		ERROR(CSS error: WTF length of value < 0 wtf impossibru);
		return CSSValues;
	}
	else if (length==0)
	{
		ERROR(CSS warning: Empty rule value);
		return CSSValues;
	}
	else
	{
		CSSValue ret;
		memset(&ret, 0, sizeof(ret));
		CSSToken current = tokens->at(start+1);
		CSSToken next = tokens->at(start+2);
		for (
			int i=start+1;
			i<=end;
			i++,
			current=tokens->at(i),
			next=tokens->at(i+1)
			)
		{
			int intVal = 12345;
			if (current.type==TOKEN_TYPE_NUMBER ||
				current.type==TOKEN_TYPE_PERCENTAGE)
			{
				intVal = current.text.toInt();
			}
			if (current.type==TOKEN_TYPE_STRING_NO_QUOTES)
			{
				if (current.text==std::string("red"))
					ret.colorValue = 0xFF000000;
				else if (current.text==std::string("green"))
					ret.colorValue = 0x00FF0000;
				else if (current.text==std::string("blue"))
					ret.colorValue = 0x0000FF00;
				else if (current.text==std::string("orange"))
					ret.colorValue = 0xFF880000;
				else if (current.text==std::string("yellow"))
					ret.colorValue = 0xFFFF0000;
				else if (current.text==std::string("purple"))
					ret.colorValue = 0xFF00FF00;
				else if (current.text==std::string("white"))
					ret.colorValue = 0xFFFFFF00;
				else if (current.text==std::string("black"))
					ret.colorValue = 0x00000000;
				else if (current.text==std::string("devil"))
					ret.colorValue = 0x66666600;
				else if (current.text==std::string("rgb") ||
						 current.text==std::string("rgba"))
				{ // rgb(0, 0, 0)
					if (next.type==TOKEN_TYPE_OPERATOR &&
						next.text=='(')
					{
						CSSToken iPlus2 = tokens->at(i+2);
						CSSToken iPlus3 = tokens->at(i+3);
						CSSToken iPlus4 = tokens->at(i+4);
						CSSToken iPlus5 = tokens->at(i+5);
						CSSToken iPlus6 = tokens->at(i+6);
						CSSToken iPlus7 = tokens->at(i+7);
						CSSToken iPlus8, iPlus9;
						if (current.text.length==4) // rgba
						{
							iPlus8 = tokens->at(i+8);
							iPlus9 = tokens->at(i+9);
						}
						i += 7;
						if (iPlus3.type==TOKEN_TYPE_OPERATOR &&
							iPlus5.type==TOKEN_TYPE_OPERATOR &&
							iPlus7.type==TOKEN_TYPE_OPERATOR &&
							iPlus3.text==',' &&
							iPlus5.text==',' &&
							(
								(	current.text.length==3 &&
									iPlus7.text==')'	)
								||
								(	current.text.length==4 &&
									iPlus7.text==',' &&
									iPlus9.type==TOKEN_TYPE_OPERATOR &&
									iPlus9.text==')' &&
									iPlus8.type==TOKEN_TYPE_NUMBER	)
							)
							&&
							iPlus2.type==TOKEN_TYPE_NUMBER &&
							iPlus4.type==TOKEN_TYPE_NUMBER &&
							iPlus6.type==TOKEN_TYPE_NUMBER)
						{
							ret.colorValue =	(0x01000000*iPlus2.text.toInt()) |
												(0x00010000*iPlus4.text.toInt()) |
												(0x00000100*iPlus6.text.toInt());
							if (current.text.length==4)
							{
								ret.colorValue |= (0x00000001*iPlus8.text.toInt());
							}
						}
						else
						{
							std::cout << RED << "CSS syntax error: expectefd (int,int,int) after rgb\n" << NOCLR;
						}
					}
					else
					{
						std::cout << RED << "CSS Yacc error: expected ( after rgb\n" << NOCLR;
					}
				}
				else goto noColor;
				ret.colorType = 1;
				goto foundValue;
				noColor:;
			}
			if (current.type==TOKEN_TYPE_NUMBER)
			{
				if (next.text-=std::string("px"))
				{
					ret.lengthType = LENGTH_TYPE_PX;
					ret.lengthValue = intVal;
					i++;
					goto foundValue;
				}
				else if (next.text-=std::string("em"))
				{
					ret.lengthType = LENGTH_TYPE_EM;
					ret.lengthValue = intVal;
					i++;
					goto foundValue;
				}
			}
			else if (current.type==TOKEN_TYPE_PERCENTAGE)
			{
				ret.lengthType = LENGTH_TYPE_PERCENT;
				ret.lengthValue = intVal;
				goto foundValue;
			}
			if (current.type==TOKEN_TYPE_NUMBER)
			{
				if (next.text-=std::string("ms"))
				{
					ret.timeType = TIME_TYPE_MS;
					ret.timeValue = current.text.toInt();
					i++;
					goto foundValue;
				}
				else if (next.text-=std::string("s"))
				{
					ret.timeType = TIME_TYPE_S;
					ret.timeValue = current.text.toInt();
					i++;
					goto foundValue;
				}
			}
			if (current.type==TOKEN_TYPE_STRING_SINGLE_QUOTES)
			{
				ret.textType = TEXT_TYPE_SINGLE_QUOTES;
				ret.textValue = current.text;
				goto foundValue;
			}
			else if (current.type==TOKEN_TYPE_STRING_DOUBLE_QUOTES)
			{
				ret.textType = TEXT_TYPE_DOUBLE_QUOTES;
				ret.textValue = current.text;
				goto foundValue;
			}
			std::cout	<< RED << "CSS Syntax error. Unexpected token "
						<< current.text << "\n" << NOCLR;
			foundValue:
			CSSValues->push_back(ret);
			memset(&ret, 0, sizeof(ret));
		}
	}
	return CSSValues;
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
					if (t.type==TOKEN_TYPE_STRING_NO_QUOTES) // Current token is a string
					{
						if (curS.subSelectors.size()>0 && curS.subSelectors.back().type!=-1) // Not an op
						{
							/*std::cout << "Encountered space operator\n";
							CSSSubSelector sub;
							sub.str1 = std::string(" ");
							sub.type = -1;
							curS.subSelectors.push_back(sub);*/
						}
						if (t.text[0]=='#')
						{
							std::cout << "Encountered ID " << t.text << "\n";
							CSSSubSelector sub;
							sub.str1 = std::string("id");
							sub.str2 = t.text.subString(1);
							sub.type = 3;
							curS.subSelectors.push_back(sub);
						}
						else if (t.text[0]=='.')
						{
							std::cout << "Encountered Class " << t.text << "\n";
							CSSSubSelector sub;
							sub.str1 = t.text.subString(1);
							sub.type = 1;
							curS.subSelectors.push_back(sub);
						}
						else
						{
							std::cout << "Encountered tag name "<< t.text << "\n";
							CSSSubSelector sub;
							sub.str1 = t.text;
							sub.type = 0;
							curS.subSelectors.push_back(sub);
						}
					}
					else if (t.type==TOKEN_TYPE_OPERATOR)
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
					if (t.type==TOKEN_TYPE_STRING_NO_QUOTES) // Current token is a string
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
					else if (t.type==TOKEN_TYPE_OPERATOR) // Current token is an operator
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
					if (t.type==TOKEN_TYPE_STRING_NO_QUOTES) // Current token is a string
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
					else if (t.type==TOKEN_TYPE_OPERATOR) // The current token is an operator
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
							std::cout << RED << "CSS Yacc error: Unexpected operator " << t.text << "\n" << NOCLR;
						}
					}
					else
					{
						std::cout << RED << "CSS Yacc fatal error: t.type=" << t.type << "\n" << NOCLR;
					}
				}
				else
				{
						std::cout << RED << "CSS Yacc fatal error: inWhatWhat=" << inWhatWhat << "\n" << NOCLR;
				}
			}
			else if (inWhat==1) // in rule before :
			{
				if (t.type==TOKEN_TYPE_STRING_NO_QUOTES) // Current token is a string
				{
					if (curC.ruleProperties.size()!=curC.ruleValues.size())
					{
						std::cout << RED << "CSS Yacc error: Unexpected string " << t.text << "\n" << NOCLR;
					}
					else
					{
						curC.ruleProperties.push_back(parseRuleName(t.text));
						std::cout << "Added rule name " << t.text << "\n";
					}
				}
				else if (t.type==TOKEN_TYPE_OPERATOR) // Current token is an op
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
				if (t.type==TOKEN_TYPE_STRING_NO_QUOTES) // Current token is a string
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
				else if (t.type==TOKEN_TYPE_OPERATOR) // Current token is an op
				{
					if (t.text==std::string(";"))
					{
						std::cout << "Encountered ; - now in rule before :\n";
						inWhat = 1;
						std::vector<CSSValue>* CSSValues = parseRuleValue(CSSTokens, ruleValueStartI, i-1);
						CSSProperty prop = curC.ruleProperties.back();
						if (prop >= COMBOS)
						{
#define PUSHV curC.ruleValues.push_back(*v)
							curC.ruleProperties.pop_back();
							// It's a combo css property, like 'border' or 'padding'
							switch (prop)
							{
								case BORDER:
								for (std::vector<CSSValue>::iterator v
									 =CSSValues->begin(); v!=CSSValues->end(); ++v)
								{
									if (v->lengthType!=LENGTH_TYPE_NOPE)
									{
						curC.ruleProperties.push_back(BORDER_TOP_WIDTH);PUSHV;
						curC.ruleProperties.push_back(BORDER_BOTTOM_WIDTH);PUSHV;
						curC.ruleProperties.push_back(BORDER_LEFT_WIDTH);PUSHV;
						curC.ruleProperties.push_back(BORDER_RIGHT_WIDTH);PUSHV;
									}
									else if (v->colorType)
									{
						curC.ruleProperties.push_back(BORDER_TOP_COLOR);PUSHV;
						curC.ruleProperties.push_back(BORDER_BOTTOM_COLOR);PUSHV;
						curC.ruleProperties.push_back(BORDER_LEFT_COLOR);PUSHV;
						curC.ruleProperties.push_back(BORDER_RIGHT_COLOR);PUSHV;
									}
									else if (v->constant)
									{
						curC.ruleProperties.push_back(BORDER_TOP_STYLE);PUSHV;
						curC.ruleProperties.push_back(BORDER_BOTTOM_STYLE);PUSHV;
						curC.ruleProperties.push_back(BORDER_LEFT_STYLE);PUSHV;
						curC.ruleProperties.push_back(BORDER_RIGHT_STYLE);PUSHV;
									}
								}
								break;
								case BORDER_TOP:
								for (std::vector<CSSValue>::iterator v
									 =CSSValues->begin(); v!=CSSValues->end(); ++v)
								{
									if (v->lengthType!=LENGTH_TYPE_NOPE)
						curC.ruleProperties.push_back(BORDER_TOP_WIDTH), PUSHV;
									else if (v->colorType)
						curC.ruleProperties.push_back(BORDER_TOP_COLOR), PUSHV;
									else if (v->constant)
						curC.ruleProperties.push_back(BORDER_TOP_STYLE), PUSHV;
								}
								break;
								case BORDER_BOTTOM:
								for (std::vector<CSSValue>::iterator v
									 =CSSValues->begin(); v!=CSSValues->end(); ++v)
								{
									if (v->lengthType!=LENGTH_TYPE_NOPE)
						curC.ruleProperties.push_back(BORDER_BOTTOM_WIDTH), PUSHV;
									else if (v->colorType)
						curC.ruleProperties.push_back(BORDER_BOTTOM_COLOR), PUSHV;
									else if (v->constant)
						curC.ruleProperties.push_back(BORDER_BOTTOM_STYLE), PUSHV;
								}
								break;
								case BORDER_LEFT:
								for (std::vector<CSSValue>::iterator v
									 =CSSValues->begin(); v!=CSSValues->end(); ++v)
								{
									if (v->lengthType!=LENGTH_TYPE_NOPE)
						curC.ruleProperties.push_back(BORDER_LEFT_WIDTH), PUSHV;
									else if (v->colorType)
						curC.ruleProperties.push_back(BORDER_LEFT_COLOR), PUSHV;
									else if (v->constant)
						curC.ruleProperties.push_back(BORDER_LEFT_STYLE), PUSHV;
								}
								break;
								case BORDER_RIGHT:
								for (std::vector<CSSValue>::iterator v
									 =CSSValues->begin(); v!=CSSValues->end(); ++v)
								{
									if (v->lengthType!=LENGTH_TYPE_NOPE)
						curC.ruleProperties.push_back(BORDER_RIGHT_STYLE), PUSHV;
									else if (v->colorType)
						curC.ruleProperties.push_back(BORDER_RIGHT_STYLE), PUSHV;
									else if (v->constant)
						curC.ruleProperties.push_back(BORDER_RIGHT_STYLE), PUSHV;
								}
								break;
								default:
								std::cout << RED << "CSS Yacc error: Todo combo " << prop << NOCLR;
								break;
							}
						}
						else
						{
							// It's a single css property, like 'border-top-width'
							if (CSSValues->size()==1)
							{
								curC.ruleValues.push_back(CSSValues->at(0));
							}
							else
							{
								std::cout << RED << "CSS Yacc error: Incorrect amount of CSS values supplied!\n" << NOCLR;
								
							}
						}
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
				curS.subSelectors.at(i).str1.print();
				std::cout << "-";
				std::cout << curS.subSelectors.at(i).str2;
				std::cout << "-";
				std::cout << curS.subSelectors.at(i).type << "\n";
			}
			i++;
		}
	}
	PRINT(cssYaccThreadFunc end);
	
	
	
	
	*yaccingCSS = 0;
	pthread_exit(NULL);
}
