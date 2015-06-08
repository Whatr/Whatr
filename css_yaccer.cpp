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

bool canTakeColor(CSSProperty prop)
{
	return
	prop == COLOR ||
	prop == BACKGROUND_COLOR ||
	prop == BORDER_BOTTOM_COLOR ||
	prop == BORDER_TOP_COLOR ||
	prop == BORDER_LEFT_COLOR ||
	prop == BORDER_RIGHT_COLOR ||
	prop == BORDER ||
	prop == BORDER_COLOR ||
	prop == BORDER_LEFT ||
	prop == BORDER_RIGHT ||
	prop == BORDER_TOP ||
	prop == BORDER_BOTTOM;
}

std::vector<CSSValue>* parseRuleValue(std::vector<CSSToken>* tokens, int start, int end, CSSProperty prop)
// The prop parameter is purely to limit the amount of string matching for constants
{
	// start points to the :
	// end points to the token before the ;
	
	bool acceptColor = canTakeColor(prop);
	
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
			if (current.type==TOKEN_TYPE_STRING_NO_QUOTES &&
				current.text[0]=='#')
			{
				if (current.text.length==4) // #ABC
				{
					char tempHex[7];
					tempHex[6] = 0;
					current.text.subString(1).copyTo(&tempHex[0]);
					tempHex[4] = tempHex[5] = tempHex[2];
					tempHex[2] = tempHex[3] = tempHex[1];
					tempHex[1] = tempHex[0];
					ret.colorValue = ((int)strtol(&tempHex[0], NULL, 16)) << 8;
					ret.colorType = 1;
					goto foundValue;
				}
				else if (current.text.length==7) // #ABCDEF
				{
					ret.colorValue = current.text.subString(1).toInt(16) << 8;
					ret.colorType = 1;
					goto foundValue;
				}
				else
				{
					std::cout << RED << "CSS syntax error: expected 3 or 6 hex characters after #\n" << NOCLR;
					
					ret.colorValue = 0xFF000000;
				}
			}
			if (current.type==TOKEN_TYPE_STRING_NO_QUOTES)
			{
				if (current.text-=std::string("initial"))
				{
					ret.constant = INITIAL;
					goto foundValue;
				}
				if (current.text-=std::string("inherit"))
				{
					ret.constant = INHERIT;
					goto foundValue;
				}
				if (acceptColor)
				{
#define evalColor(x,y) if(current.text.equalsIgnoreCase(std::string(x),'-'))ret.colorValue=y
					     evalColor("AliceBlue", 0xF0F8FF00);
					else evalColor("AntiqueWhite", 0xFAEBD700);
					else evalColor("Aqua", 0x00FFFF00);
					else evalColor("Aquamarine", 0x7FFFD400);
					else evalColor("Azure", 0xF0FFFF00);
					else evalColor("Beige", 0xF5F5DC00);
					else evalColor("Bisque", 0xFFE4C400);
					else evalColor("Black", 0x00000000);
					else evalColor("BlanchedAlmond", 0xFFEBCD00);
					else evalColor("Blue", 0x0000FF00);
					else evalColor("BlueViolet", 0x8A2BE200);
					else evalColor("Brown", 0xA52A2A00);
					else evalColor("BurlyWood", 0xDEB88700);
					else evalColor("CadetBlue", 0x5F9EA000);
					else evalColor("Chartreuse", 0x7FFF0000);
					else evalColor("Chocolate", 0xD2691E00);
					else evalColor("Coral", 0xFF7F5000);
					else evalColor("CornflowerBlue", 0x6495ED00);
					else evalColor("Cornsilk", 0xFFF8DC00);
					else evalColor("Crimson", 0xDC143C00);
					else evalColor("Cyan", 0x00FFFF00);
					else evalColor("DarkBlue", 0x00008B00);
					else evalColor("DarkCyan", 0x008B8B00);
					else evalColor("DarkGoldenRod", 0xB8860B00);
					else evalColor("DarkGray", 0xA9A9A900);
					else evalColor("DarkGreen", 0x00640000);
					else evalColor("DarkKhaki", 0xBDB76B00);
					else evalColor("DarkMagenta", 0x8B008B00);
					else evalColor("DarkOliveGreen", 0x556B2F00);
					else evalColor("DarkOrange", 0xFF8C0000);
					else evalColor("DarkOrchid", 0x9932CC00);
					else evalColor("DarkRed", 0x8B000000);
					else evalColor("DarkSalmon", 0xE9967A00);
					else evalColor("DarkSeaGreen", 0x8FBC8F00);
					else evalColor("DarkSlateBlue", 0x483D8B00);
					else evalColor("DarkSlateGray", 0x2F4F4F00);
					else evalColor("DarkTurquoise", 0x00CED100);
					else evalColor("DarkViolet", 0x9400D300);
					else evalColor("DeepPink", 0xFF149300);
					else evalColor("DeepSkyBlue", 0x00BFFF00);
					else evalColor("DimGray", 0x69696900);
					else evalColor("DodgerBlue", 0x1E90FF00);
					else evalColor("FireBrick", 0xB2222200);
					else evalColor("FloralWhite", 0xFFFAF000);
					else evalColor("ForestGreen", 0x228B2200);
					else evalColor("Fuchsia", 0xFF00FF00);
					else evalColor("Gainsboro", 0xDCDCDC00);
					else evalColor("GhostWhite", 0xF8F8FF00);
					else evalColor("Gold", 0xFFD70000);
					else evalColor("GoldenRod", 0xDAA52000);
					else evalColor("Gray", 0x80808000);
					else evalColor("Green", 0x00800000);
					else evalColor("GreenYellow", 0xADFF2F00);
					else evalColor("HoneyDew", 0xF0FFF000);
					else evalColor("HotPink", 0xFF69B400);
					else evalColor("IndianRed ", 0xCD5C5C00);
					else evalColor("Indigo  ", 0x4B008200);
					else evalColor("Ivory", 0xFFFFF000);
					else evalColor("Khaki", 0xF0E68C00);
					else evalColor("Lavender", 0xE6E6FA00);
					else evalColor("LavenderBlush", 0xFFF0F500);
					else evalColor("LawnGreen", 0x7CFC0000);
					else evalColor("LemonChiffon", 0xFFFACD00);
					else evalColor("LightBlue", 0xADD8E600);
					else evalColor("LightCoral", 0xF0808000);
					else evalColor("LightCyan", 0xE0FFFF00);
					else evalColor("LightGoldenRodYellow", 0xFAFAD200);
					else evalColor("LightGray", 0xD3D3D300);
					else evalColor("LightGreen", 0x90EE9000);
					else evalColor("LightPink", 0xFFB6C100);
					else evalColor("LightSalmon", 0xFFA07A00);
					else evalColor("LightSeaGreen", 0x20B2AA00);
					else evalColor("LightSkyBlue", 0x87CEFA00);
					else evalColor("LightSlateGray", 0x77889900);
					else evalColor("LightSteelBlue", 0xB0C4DE00);
					else evalColor("LightYellow", 0xFFFFE000);
					else evalColor("Lime", 0x00FF0000);
					else evalColor("LimeGreen", 0x32CD3200);
					else evalColor("Linen", 0xFAF0E600);
					else evalColor("Magenta", 0xFF00FF00);
					else evalColor("Maroon", 0x80000000);
					else evalColor("MediumAquaMarine", 0x66CDAA00);
					else evalColor("MediumBlue", 0x0000CD00);
					else evalColor("MediumOrchid", 0xBA55D300);
					else evalColor("MediumPurple", 0x9370DB00);
					else evalColor("MediumSeaGreen", 0x3CB37100);
					else evalColor("MediumSlateBlue", 0x7B68EE00);
					else evalColor("MediumSpringGreen", 0x00FA9A00);
					else evalColor("MediumTurquoise", 0x48D1CC00);
					else evalColor("MediumVioletRed", 0xC7158500);
					else evalColor("MidnightBlue", 0x19197000);
					else evalColor("MintCream", 0xF5FFFA00);
					else evalColor("MistyRose", 0xFFE4E100);
					else evalColor("Moccasin", 0xFFE4B500);
					else evalColor("NavajoWhite", 0xFFDEAD00);
					else evalColor("Navy", 0x00008000);
					else evalColor("OldLace", 0xFDF5E600);
					else evalColor("Olive", 0x80800000);
					else evalColor("OliveDrab", 0x6B8E2300);
					else evalColor("Orange", 0xFFA50000);
					else evalColor("OrangeRed", 0xFF450000);
					else evalColor("Orchid", 0xDA70D600);
					else evalColor("PaleGoldenRod", 0xEEE8AA00);
					else evalColor("PaleGreen", 0x98FB9800);
					else evalColor("PaleTurquoise", 0xAFEEEE00);
					else evalColor("PaleVioletRed", 0xDB709300);
					else evalColor("PapayaWhip", 0xFFEFD500);
					else evalColor("PeachPuff", 0xFFDAB900);
					else evalColor("Peru", 0xCD853F00);
					else evalColor("Pink", 0xFFC0CB00);
					else evalColor("Plum", 0xDDA0DD00);
					else evalColor("PowderBlue", 0xB0E0E600);
					else evalColor("Purple", 0x80008000);
					else evalColor("RebeccaPurple", 0x66339900);
					else evalColor("Red", 0xFF000000);
					else evalColor("RosyBrown", 0xBC8F8F00);
					else evalColor("RoyalBlue", 0x4169E100);
					else evalColor("SaddleBrown", 0x8B451300);
					else evalColor("Salmon", 0xFA807200);
					else evalColor("SandyBrown", 0xF4A46000);
					else evalColor("SeaGreen", 0x2E8B5700);
					else evalColor("SeaShell", 0xFFF5EE00);
					else evalColor("Sienna", 0xA0522D00);
					else evalColor("Silver", 0xC0C0C000);
					else evalColor("SkyBlue", 0x87CEEB00);
					else evalColor("SlateBlue", 0x6A5ACD00);
					else evalColor("SlateGray", 0x70809000);
					else evalColor("Snow", 0xFFFAFA00);
					else evalColor("SpringGreen", 0x00FF7F00);
					else evalColor("SteelBlue", 0x4682B400);
					else evalColor("Tan", 0xD2B48C00);
					else evalColor("Teal", 0x00808000);
					else evalColor("Thistle", 0xD8BFD800);
					else evalColor("Tomato", 0xFF634700);
					else evalColor("Turquoise", 0x40E0D000);
					else evalColor("Violet", 0xEE82EE00);
					else evalColor("Wheat", 0xF5DEB300);
					else evalColor("White", 0xFFFFFF00);
					else evalColor("WhiteSmoke", 0xF5F5F500);
					else evalColor("Yellow", 0xFFFF0000);
					else evalColor("YellowGreen", 0x9ACD32);
					else evalColor("devil", 0x66666600);
					else goto notAColorName;
				}
				else goto notAColorName;
				// If it's a color name:
				ret.colorType = 1;
				goto foundValue;
				
				// Else:
				notAColorName:
				if (prop==BORDER ||
					prop==BORDER_STYLE ||
					prop==BORDER_LEFT ||
					prop==BORDER_BOTTOM ||
					prop==BORDER_RIGHT ||
					prop==BORDER_TOP ||
					prop==BORDER_LEFT_STYLE ||
					prop==BORDER_RIGHT_STYLE ||
					prop==BORDER_BOTTOM_STYLE ||
					prop==BORDER_TOP_STYLE)
				{
					     if (current.text-=std::string("none"))
						ret.constant = NONE_LINE_STYLE;
					else if (current.text-=std::string("hidden"))
						ret.constant = HIDDEN;
					else if (current.text-=std::string("dotted"))
						ret.constant = DOTTED;
					else if (current.text-=std::string("dashed"))
						ret.constant = DASHED;
					else if (current.text-=std::string("solid"))
						ret.constant = SOLID;
					else if (current.text-=std::string("double"))
						ret.constant = DOUBLE;
					else if (current.text-=std::string("groove"))
						ret.constant = GROOVE;
					else if (current.text-=std::string("ridge"))
						ret.constant = RIDGE;
					else if (current.text-=std::string("inset"))
						ret.constant = INSET;
					else if (current.text-=std::string("outset"))
						ret.constant = OUTSET;
					else goto notALineStyle;
				}
				else goto notALineStyle;
				// If it's a line style:
				goto foundValue;
				
				// Else:
				notALineStyle:
				if (prop==LIST_STYLE ||
					prop==LIST_STYLE_POSITION)
				{
					     if (current.text-=std::string("inside"))
						ret.constant = INSIDE;
					else if (current.text-=std::string("outside"))
						ret.constant = OUTSIDE;
					else goto notAListStylePosition;
				}
				else goto notAListStylePosition;
				// If it's a list style position:
				goto foundValue;
				
				// Else:
				notAListStylePosition:
				if (prop==LIST_STYLE ||
					prop==LIST_STYLE_TYPE)
				{
					     if (current.text-=std::string("disc"))
						ret.constant = DISC;
					else if (current.text-=std::string("armenian"))
						ret.constant = ARMENIAN;
					else if (current.text-=std::string("circle"))
						ret.constant = CIRCLE;
					else if (current.text-=std::string("cjk-ideographic")) // wtf
						ret.constant = CJK_IDEOGRAPHIC; // lol wtf
					else if (current.text-=std::string("decimal"))
						ret.constant = DECIMAL;
					else if (current.text-=std::string("decimal-leading-zero"))
						ret.constant = DECIMAL_LEADING_ZERO;
					else if (current.text-=std::string("georgian"))
						ret.constant = GEORGIAN;
					else if (current.text-=std::string("hebrew"))
						ret.constant = HEBREW;
					else if (current.text-=std::string("hiragana"))
						ret.constant = HIRAGANA;
					else if (current.text-=std::string("hiragana-iroha"))
						ret.constant = HIRAGANA_IROHA;
					else if (current.text-=std::string("katakana"))
						ret.constant = KATAKANA;
					else if (current.text-=std::string("katakana-iroha"))
						ret.constant = KATAKANA_IROHA;
					else if (current.text-=std::string("lower-alpha"))
						ret.constant = LOWER_ALPHA;
					else if (current.text-=std::string("lower-greek"))
						ret.constant = LOWER_GREEK;
					else if (current.text-=std::string("lower-latin"))
						ret.constant = LOWER_LATIN;
					else if (current.text-=std::string("lower-roman"))
						ret.constant = LOWER_ROMAN;
					else if (current.text-=std::string("none"))
						ret.constant = NONE_LIST_STYLE_TYPE;
					else if (current.text-=std::string("square"))
						ret.constant = SQUARE;
					else if (current.text-=std::string("upper-alpha"))
						ret.constant = UPPER_ALPHA;
					else if (current.text-=std::string("upper-latin"))
						ret.constant = UPPER_LATIN;
					else if (current.text-=std::string("upper-roman"))
						ret.constant = UPPER_ROMAN;
					else goto notAListStyleType;
				}
				else goto notAListStyleType;
				// If it's a list style type
				goto foundValue;
				
				// Else
				notAListStyleType:
				if (prop==BACKGROUND ||
					prop==BACKGROUND_REPEAT)
				{
					     if (current.text-=std::string("repeat"))
						ret.constant = REPEAT;
					else if (current.text-=std::string("repeat-x"))
						ret.constant = REPEAT_X;
					else if (current.text-=std::string("repeat-y"))
						ret.constant = REPEAT_Y;
					else if (current.text-=std::string("no-repeat"))
						ret.constant = NO_REPEAT;
					else if (current.text-=std::string("round"))
						ret.constant = ROUND;
					else if (current.text-=std::string("space"))
						ret.constant = SPACE;
					else goto notABackgroundRepeat;
				}
				else goto notABackgroundRepeat;
				// If it's a background repeat
				goto foundValue;
				
				// Else
				notABackgroundRepeat:
				if (prop==BACKGROUND ||
					prop==BACKGROUND_ATTACHMENT)
				{
					     if (current.text-=std::string("scroll"))
						ret.constant = SCROLL;
					else if (current.text-=std::string("local"))
						ret.constant = LOCAL;
					else if (current.text-=std::string("fixed"))
						ret.constant = FIXED;
					else goto notABackgroundAttachment;
				}
				else goto notABackgroundAttachment;
				// If it's a background attachment
				goto foundValue;
				
				// Else
				notABackgroundAttachment:
				if (prop==VERTICAL_ALIGN)
				{
					     if (current.text-=std::string("baseline"))
						ret.constant = BASELINE;
					else if (current.text-=std::string("top"))
						ret.constant = TOP;
					else if (current.text-=std::string("text-top"))
						ret.constant = TEXT_TOP;
					else if (current.text-=std::string("text-bottom"))
						ret.constant = TEXT_BOTTOM;
					else if (current.text-=std::string("super"))
						ret.constant = SUPER;
					else if (current.text-=std::string("sub"))
						ret.constant = SUB;
					else if (current.text-=std::string("middle"))
						ret.constant = MIDDLE;
					else if (current.text-=std::string("bottom"))
						ret.constant = BOTTOM;
					else goto notAVerticalAlign;
				}
				else goto notAVerticalAlign;
				// If it's a clear
				goto foundValue;
				
				// Else
				notAVerticalAlign:
				if (prop==FLOAT)
				{
					     if (current.text-=std::string("none"))
						ret.constant = NONE_FLOAT;
					else if (current.text-=std::string("left"))
						ret.constant = LEFT_FLOAT;
					else if (current.text-=std::string("right"))
						ret.constant = RIGHT_FLOAT;
					else goto notAFloat;
				}
				else goto notAFloat;
				// If it's a clear
				goto foundValue;
				
				// Else
				notAFloat:
				if (prop==CLEAR)
				{
					     if (current.text-=std::string("none"))
						ret.constant = NONE_CLEAR;
					else if (current.text-=std::string("left"))
						ret.constant = LEFT_CLEAR;
					else if (current.text-=std::string("right"))
						ret.constant = RIGHT_CLEAR;
					else if (current.text-=std::string("both"))
						ret.constant = BOTH;
					else goto notAClear;
				}
				else goto notAClear;
				// If it's a clear
				goto foundValue;
				
				// Else
				notAClear:
				if (prop==TEXT_INDENT)
				{
					     if (current.text-=std::string("hanging"))
						ret.constant = HANGING;
					else if (current.text-=std::string("each-line"))
						ret.constant = EACH_LINE;
					else goto notATextIndent;
				}
				else goto notATextIndent;
				// If it's a text ident
				goto foundValue;
				
				// Else
				notATextIndent:
				if (prop==FONT ||
					prop==FONT_STYLE)
				{
					     if (current.text-=std::string("normal"))
						ret.constant = NORMAL_FONT_STYLE;
					else if (current.text-=std::string("oblique"))
						ret.constant = OBLIQUE;
					else if (current.text-=std::string("italic"))
						ret.constant = ITALIC;
					else goto notAFontStyle;
				}
				else goto notAFontStyle;
				// If it's a font style
				goto foundValue;
				
				// Else
				notAFontStyle:
				if (prop==FONT ||
					prop==FONT_WEIGHT)
				{
					     if (current.text-=std::string("normal"))
						ret.constant = NORMAL_FONT_WEIGHT;
					else if (current.text-=std::string("lighter"))
						ret.constant = LIGHTER;
					else if (current.text-=std::string("bolder"))
						ret.constant = BOLDER;
					else if (current.text-=std::string("bold"))
						ret.constant = BOLD;
					else goto notAFontWeight;
				}
				else goto notAFontWeight;
				// If it's a font weight
				goto foundValue;
				
				// Else
				notAFontWeight:
				if (prop==FONT ||
					prop==FONT_VARIANT)
				{
					     if (current.text-=std::string("normal"))
						ret.constant = NORMAL_FONT_VARIANT;
					else if (current.text-=std::string("unicase"))
						ret.constant = UNICASE;
					else if (current.text-=std::string("titling-caps"))
						ret.constant = TITLING_CAPS;
					else if (current.text-=std::string("small-caps"))
						ret.constant = SMALL_CAPS;
					else if (current.text-=std::string("petite-caps"))
						ret.constant = PETITE_CAPS;
					else if (current.text-=std::string("all-small-caps"))
						ret.constant = ALL_SMALL_CAPS;
					else if (current.text-=std::string("all-petite-caps"))
						ret.constant = ALL_PETITE_CAPS;
					else goto notAFontVariant;
				}
				else goto notAFontVariant;
				// If it's a font variant
				goto foundValue;
				
				// Else
				notAFontVariant:
				if (prop==FONT ||
					prop==FONT_SIZE)
				{
					     if (current.text-=std::string("medium"))
						ret.constant = MEDIUM;
					else if (current.text-=std::string("xx-small"))
						ret.constant = XX_SMALL;
					else if (current.text-=std::string("xx-large"))
						ret.constant = XX_LARGE;
					else if (current.text-=std::string("x-small"))
						ret.constant = X_SMALL;
					else if (current.text-=std::string("x-large"))
						ret.constant = X_LARGE;
					else if (current.text-=std::string("smaller"))
						ret.constant = SMALLER;
					else if (current.text-=std::string("small"))
						ret.constant = SMALL;
					else if (current.text-=std::string("larger"))
						ret.constant = LARGER;
					else if (current.text-=std::string("large"))
						ret.constant = LARGE;
					else goto notAFontSize;
				}
				else goto notAFontSize;
				// If it's a font size
				goto foundValue;
				
				// Else
				notAFontSize:
				if (prop==TEXT_DECORATION)
				{
					     if (current.text-=std::string("none"))
						ret.constant = NONE_TEXT_DECORATION;
					else if (current.text-=std::string("underline"))
						ret.constant = UNDERLINE;
					else if (current.text-=std::string("overline"))
						ret.constant = OVERLINE;
					else if (current.text-=std::string("line-through"))
						ret.constant = LINE_THROUGH;
					else goto notATextDecoration;
				}
				else goto notATextDecoration;
				// If it's a text decoration
				goto foundValue;
				
				// Else
				notATextDecoration:
				if (prop==WHITE_SPACE ||
					prop==WORD_SPACING)
				{
					     if (current.text-=std::string("normal"))
						ret.constant = NORMAL;
					else if (prop==WORD_SPACING) goto notAWhiteSpace;
					else if (current.text-=std::string("pre-wrap"))
						ret.constant = PRE_WRAP;
					else if (current.text-=std::string("pre-line"))
						ret.constant = PRE_LINE;
					else if (current.text-=std::string("pre"))
						ret.constant = PRE;
					else if (current.text-=std::string("nowrap"))
						ret.constant = NOWRAP;
					else goto notAWhiteSpace;
				}
				else goto notAWhiteSpace;
				// If it's a white space or word spacing
				goto foundValue;
				
				// Else
				notAWhiteSpace:
				if (prop==TEXT_TRANSFORM)
				{
					     if (current.text-=std::string("none"))
						ret.constant = NONE_TEXT_TRANSFORM;
					else if (current.text-=std::string("uppercase"))
						ret.constant = UPPERCASE;
					else if (current.text-=std::string("lowercase"))
						ret.constant = LOWERCASE;
					else if (current.text-=std::string("full-width"))
						ret.constant = FULL_WIDTH;
					else if (current.text-=std::string("capitalize"))
						ret.constant = CAPITALIZE;
					else goto notATextTransform;
				}
				else goto notATextTransform;
				// If it's a clear
				goto foundValue;
				
				// Else
				notATextTransform:
				if (prop==TEXT_ALIGN)
				{
					     if (current.text-=std::string("start"))
						ret.constant = START_TEXT_ALIGN;
					else if (current.text-=std::string("end"))
						ret.constant = END_TEXT_ALIGN;
					else if (current.text-=std::string("left"))
						ret.constant = LEFT_TEXT_ALIGN;
					else if (current.text-=std::string("right"))
						ret.constant = RIGHT_TEXT_ALIGN;
					else if (current.text-=std::string("match-parent"))
						ret.constant = MATCH_PARENT;
					else if (current.text-=std::string("justify"))
						ret.constant = JUSTIFY;
					else if (current.text-=std::string("center"))
						ret.constant = CENTER_TEXT_ALIGN;
					else goto notATextAlign;
				}
				else goto notATextAlign;
				// If it's a text-align
				goto foundValue;
				
				// Else
				notATextAlign:
				if (prop==DISPLAY)
				{
					     if (current.text-=std::string("inline"))
						ret.constant = INLINE;
					else if (current.text-=std::string("table-row-group"))
						ret.constant = TABLE_ROW_GROUP;
					else if (current.text-=std::string("table-row"))
						ret.constant = TABLE_ROW;
					else if (current.text-=std::string("table-header-group"))
						ret.constant = TABLE_HEADER_GROUP;
					else if (current.text-=std::string("table-footer-group"))
						ret.constant = TABLE_FOOTER_GROUP;
					else if (current.text-=std::string("table-column-group"))
						ret.constant = TABLE_COLUMN_GROUP;
					else if (current.text-=std::string("table-column"))
						ret.constant = TABLE_COLUMN;
					else if (current.text-=std::string("table-cell"))
						ret.constant = TABLE_CELL;
					else if (current.text-=std::string("table-caption"))
						ret.constant = TABLE_CAPTION;
					else if (current.text-=std::string("table"))
						ret.constant = TABLE;
					else if (current.text-=std::string("run-in"))
						ret.constant = RUN_IN;
					else if (current.text-=std::string("none"))
						ret.constant = NONE_DISPLAY;
					else if (current.text-=std::string("list-item"))
						ret.constant = LIST_ITEM;
					else if (current.text-=std::string("inline-table"))
						ret.constant = INLINE_TABLE;
					else if (current.text-=std::string("inline-flex"))
						ret.constant = INLINE_FLEX;
					else if (current.text-=std::string("inline-block"))
						ret.constant = INLINE_BLOCK;
					else if (current.text-=std::string("flex"))
						ret.constant = FLEX;
					else if (current.text-=std::string("container"))
						ret.constant = CONTAINER;
					else if (current.text-=std::string("compact"))
						ret.constant = COMPACT;
					else if (current.text-=std::string("block"))
						ret.constant = BLOCK;
					else goto notADisplay;
				}
				else goto notADisplay;
				// If it's a display
				goto foundValue;
				
				// Else
				notADisplay:
				if (prop==BACKGROUND ||
					prop==BACKGROUND_POSITION)
				{
					if (current.text-=std::string("left"))
					{
						ret.lengthType = LENGTH_TYPE_PERCENT;
						ret.lengthValue = 0;
					}
					else if (current.text-=std::string("center"))
					{
						ret.lengthType = LENGTH_TYPE_PERCENT;
						ret.lengthValue = 50;
					}
					else if (current.text-=std::string("right"))
					{
						ret.lengthType = LENGTH_TYPE_PERCENT;
						ret.lengthValue = 100;
					}
					else goto notABackgroundPosition;
				}
				else goto notABackgroundPosition;
				// If it's a background position
				goto foundValue;
				
				// Else
				notABackgroundPosition:
				if (current.text==std::string("url"))
				{
					CSSToken iPlus2 = tokens->at(i+2);
					CSSToken iPlus3 = tokens->at(i+3);
					if (next.type==TOKEN_TYPE_OPERATOR &&
						next.text=='(' &&
						(iPlus2.type==TOKEN_TYPE_STRING_SINGLE_QUOTES ||
						 iPlus2.type==TOKEN_TYPE_STRING_DOUBLE_QUOTES) &&
						iPlus3.type==TOKEN_TYPE_OPERATOR &&
						iPlus3.text==')')
					{
						ret.textType = TEXT_TYPE_URL;
						ret.textValue = iPlus2.text;
						i += 3;
						goto foundValue;
					}
					else
					{
						std::cout << RED << "CSS syntax error: expected ('string') after url\n" << NOCLR;
					}
				}
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
							i += 9;
						}
						else i += 7;
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
							if (current.text.length==3) std::cout << RED << "CSS syntax error: expected (int,int,int) after rgb\n" << NOCLR;
							else std::cout << RED << "CSS syntax error: expected (int,int,int,int) after rgba\n" << NOCLR;
						}
					}
					else
					{
						std::cout << RED << "CSS syntax error: expected ( after rgb\n" << NOCLR;
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
				else if (next.text-=std::string("pt"))
				{
					ret.lengthType = LENGTH_TYPE_PT;
					ret.lengthValue = intVal;
					i++;
					goto foundValue;
				}
				else
				{
					ret.lengthType = LENGTH_TYPE_UNKNOWN;
					ret.lengthValue = intVal;
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
						CSSProperty prop = curC.ruleProperties.back();
						std::vector<CSSValue>* CSSValues = parseRuleValue(CSSTokens, ruleValueStartI, i-1, prop);
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
									else std::cout << "TODO JKL$AJ%KJASKLDFJ\n";
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
									else std::cout << "TODO JKL$AJ%KJASKLDFJ\n";
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
									else std::cout << "TODO JKL$AJ%KJASKLDFJ\n";
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
									else std::cout << "TODO JKL$AJ%KJASKLDFJ\n";
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
									else std::cout << "TODO JKL$AJ%KJASKLDFJ\n";
								}
								break;
								case BACKGROUND:
								for (std::vector<CSSValue>::iterator v
									 =CSSValues->begin(); v!=CSSValues->end(); ++v)
								{
									if (v->colorType)
						curC.ruleProperties.push_back(BACKGROUND_COLOR), PUSHV;
									else if (v->textType==TEXT_TYPE_URL)
						curC.ruleProperties.push_back(BACKGROUND_IMAGE), PUSHV;
									else std::cout << "TODO JKL$AJ%KJASKLDFJ\n";
								}
								case BORDER_COLOR:
								case BORDER_WIDTH:
								case BORDER_STYLE:
								case MARGIN:
								case PADDING:{
								CSSProperty top, bottom, left, right;
								if (prop==MARGIN)
									(top = MARGIN_TOP),
									(bottom = MARGIN_BOTTOM),
									(left = MARGIN_LEFT),
									(right = MARGIN_RIGHT);
								if (prop==PADDING)
									(top = PADDING_TOP),
									(bottom = PADDING_BOTTOM),
									(left = PADDING_LEFT),
									(right = PADDING_RIGHT);
								if (prop==BORDER_COLOR)
									(top = BORDER_TOP_COLOR),
									(bottom = BORDER_BOTTOM_COLOR),
									(left = BORDER_LEFT_COLOR),
									(right = BORDER_RIGHT_COLOR);
								if (prop==BORDER_WIDTH)
									(top = BORDER_TOP_WIDTH),
									(bottom = BORDER_BOTTOM_WIDTH),
									(left = BORDER_LEFT_WIDTH),
									(right = BORDER_RIGHT_WIDTH);
								if (prop==BORDER_STYLE)
									(top = BORDER_TOP_STYLE),
									(bottom = BORDER_BOTTOM_STYLE),
									(left = BORDER_LEFT_STYLE),
									(right = BORDER_RIGHT_STYLE);
								
								if (CSSValues->size()==1) // TRBL
						curC.ruleProperties.push_back(top),
						curC.ruleProperties.push_back(right),
						curC.ruleProperties.push_back(bottom),
						curC.ruleProperties.push_back(left),
						curC.ruleValues.push_back((*CSSValues)[0]),
						curC.ruleValues.push_back((*CSSValues)[0]),
						curC.ruleValues.push_back((*CSSValues)[0]),
						curC.ruleValues.push_back((*CSSValues)[0]);
								else if (CSSValues->size()==2) // TB RL
						curC.ruleProperties.push_back(top),
						curC.ruleProperties.push_back(bottom),
						curC.ruleProperties.push_back(right),
						curC.ruleProperties.push_back(left),
						curC.ruleValues.push_back((*CSSValues)[0]),
						curC.ruleValues.push_back((*CSSValues)[0]),
						curC.ruleValues.push_back((*CSSValues)[1]),
						curC.ruleValues.push_back((*CSSValues)[1]);
								else if (CSSValues->size()==3) // T RL B
						curC.ruleProperties.push_back(top),
						curC.ruleProperties.push_back(right),
						curC.ruleProperties.push_back(left),
						curC.ruleProperties.push_back(bottom),
						curC.ruleValues.push_back((*CSSValues)[0]),
						curC.ruleValues.push_back((*CSSValues)[1]),
						curC.ruleValues.push_back((*CSSValues)[1]),
						curC.ruleValues.push_back((*CSSValues)[2]);
								else if (CSSValues->size()==4) // T R B L
						curC.ruleProperties.push_back(top),
						curC.ruleProperties.push_back(right),
						curC.ruleProperties.push_back(bottom),
						curC.ruleProperties.push_back(left),
						curC.ruleValues.push_back((*CSSValues)[0]),
						curC.ruleValues.push_back((*CSSValues)[1]),
						curC.ruleValues.push_back((*CSSValues)[2]),
						curC.ruleValues.push_back((*CSSValues)[3]);
								else
								{
									std::cout << "CSS syntax error: property has to supply 1, 2, 3 or 4 values. Not " << CSSValues->size() << "\n";
								}
								}break;
								case LIST_STYLE:
								for (std::vector<CSSValue>::iterator v
									 =CSSValues->begin(); v!=CSSValues->end(); ++v)
								{
									if (v->textType==TEXT_TYPE_NO_QUOTES)
									{
										if (v->constant==INSIDE ||
											v->constant==OUTSIDE)
										// list-style-position
						curC.ruleProperties.push_back(LIST_STYLE_POSITION), PUSHV;
										else // list-style-type
						curC.ruleProperties.push_back(LIST_STYLE_TYPE), PUSHV;
									}
									else if (v->textType==TEXT_TYPE_URL)
									{
						curC.ruleProperties.push_back(LIST_STYLE_IMAGE), PUSHV;
									}
									else
									{
										std::cout << "CSS syntax error: list-style only takes specific words and one url\n";
									}
								}
								break;
								case FONT:
								for (std::vector<CSSValue>::iterator v
									 =CSSValues->begin(); v!=CSSValues->end(); ++v)
								{
									if (v->textType==TEXT_TYPE_NO_QUOTES)
									{
										if (v->constant>_FONT_STYLE_START &&
										    v->constant<_FONT_STYLE_END)
						curC.ruleProperties.push_back(FONT_STYLE), PUSHV;
										else if (v->constant>_FONT_VARIANT_START &&
										         v->constant<_FONT_VARIANT_END)
						curC.ruleProperties.push_back(FONT_VARIANT), PUSHV;
										else if (v->constant>_FONT_WEIGHT_START &&
										         v->constant<_FONT_WEIGHT_END)
						curC.ruleProperties.push_back(FONT_WEIGHT), PUSHV;
										else
										{
											std::cout << RED << "CSS syntax error: invalid word in font property\n" << NOCLR;
										}
									}
									else if (v->colorType)
									{
						curC.ruleProperties.push_back(COLOR), PUSHV;
									}
									else if (v->lengthType!=LENGTH_TYPE_NOPE)
									{
										if (v->lengthType==LENGTH_TYPE_UNKNOWN &&
											(
												v->lengthValue==100 ||
												v->lengthValue==200 ||
												v->lengthValue==300 ||
												v->lengthValue==400 ||
												v->lengthValue==500 ||
												v->lengthValue==600 ||
												v->lengthValue==700 ||
												v->lengthValue==800 ||
												v->lengthValue==900
											)
										)
						curC.ruleProperties.push_back(FONT_WEIGHT), PUSHV;
										else
						curC.ruleProperties.push_back(FONT_SIZE), PUSHV;
									}
									else
									{
										std::cout << "CSS syntax error: font property received something it doesn't like\n";
									}
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
