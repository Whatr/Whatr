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

#ifndef css_values_included
#define css_values_included yes

#include <iostream>
#include "const_str.h"

enum TimeType
{
	TIME_TYPE_NOPE=0,
	TIME_TYPE_MS,
	TIME_TYPE_S,
};
enum TextType
{
	TEXT_TYPE_NOPE=0,
	TEXT_TYPE_NO_QUOTES,
	TEXT_TYPE_SINGLE_QUOTES,
	TEXT_TYPE_DOUBLE_QUOTES,
	TEXT_TYPE_URL,
};
enum LengthType
{
	LENGTH_TYPE_NOPE=0,
	LENGTH_TYPE_PX,
	LENGTH_TYPE_EM,
	LENGTH_TYPE_PERCENT,
};

enum CSSConstant
{
	NOPE=0,
	
	// Global
	INITIAL,
	INHERIT,
	
	// border-style
	NONE_LINE_STYLE,
	HIDDEN,
	DOTTED,
	DASHED,
	SOLID,
	DOUBLE,
	GROOVE,
	RIDGE,
	INSET,
	OUTSET,
	
	// list-style-position
	INSIDE,
	OUTSIDE,
	
	// list-style-type
	DISC,
	ARMENIAN,
	CIRCLE,
	CJK_IDEOGRAPHIC,
	DECIMAL,
	DECIMAL_LEADING_ZERO,
	GEORGIAN,
	HEBREW,
	HIRAGANA,
	HIRAGANA_IROHA,
	KATAKANA,
	KATAKANA_IROHA,
	LOWER_ALPHA,
	LOWER_GREEK,
	LOWER_LATIN,
	LOWER_ROMAN,
	NONE_LIST_STYLE_TYPE,
	SQUARE,
	UPPER_ALPHA,
	UPPER_LATIN,
	UPPER_ROMAN,
	
	// background-repeat
	REPEAT,
	SPACE,
	ROUND,
	REPEAT_Y,
	REPEAT_X,
	NO_REPEAT,
	
	// background-attachment
	SCROLL,
	LOCAL,
	FIXED,
	
	// clear
	NONE_CLEAR,
	RIGHT_CLEAR,
	LEFT_CLEAR,
	BOTH,
	
	// display
	INLINE,
	TABLE_ROW_GROUP,
	TABLE_ROW,
	TABLE_HEADER_GROUP,
	TABLE_FOOTER_GROUP,
	TABLE_COLUMN_GROUP,
	TABLE_COLUMN,
	TABLE_CELL,
	TABLE_CAPTION,
	TABLE,
	RUN_IN,
	NONE_DISPLAY,
	LIST_ITEM,
	INLINE_TABLE,
	INLINE_FLEX,
	INLINE_BLOCK,
	FLEX,
	CONTAINER,
	COMPACT,
	BLOCK,
	
	// float
	NONE_FLOAT,
	RIGHT_FLOAT,
	LEFT_FLOAT,
	
	// vertical-align
	BASELINE,
	TOP,
	TEXT_TOP,
	TEXT_BOTTOM,
	SUPER,
	SUB,
	MIDDLE,
	BOTTOM,
	
	// text-align
	START_TEXT_ALIGN,
	END_TEXT_ALIGN,
	LEFT_TEXT_ALIGN,
	RIGHT_TEXT_ALIGN,
	MATCH_PARENT,
	JUSTIFY,
	CENTER_TEXT_ALIGN,
	
	// text-transform
	NONE_TEXT_TRANSFORM,
	UPPERCASE,
	LOWERCASE,
	FULL_WIDTH,
	CAPITALIZE,
	
	// text-indent
	HANGING,
	EACH_LINE,
	
	// white-space
	NORMAL, // <-- also for word-spacing
	PRE_WRAP,
	PRE_LINE,
	PRE,
	NOWRAP,
	
	// text-decoration
	NONE_TEXT_DECORATION,
	UNDERLINE,
	OVERLINE,
	LINE_THROUGH,
	
	// font-size
	MEDIUM,
	XX_SMALL,
	XX_LARGE,
	X_SMALL,
	X_LARGE,
	SMALLER,
	SMALL,
	LARGER,
	LARGE,
	
	// font-style
	NORMAL_FONT_STYLE,
	OBLIQUE,
	ITALIC,
	
	// font-variant
	NORMAL_FONT_VARIANT,
	UNICASE,
	TITLING_CAPS,
	SMALL_CAPS,
	PETITE_CAPS,
	ALL_SMALL_CAPS,
	ALL_PETITE_CAPS,
	
	// font-weight
	NORMAL_FONT_WEIGHT,
	LIGHTER,
	BOLDER,
	BOLD,
};

struct CSSValue
{
	TextType textType;
	ConstStr textValue;
	
	LengthType lengthType;
	double lengthValue;
	
	TimeType timeType;
	double timeValue;
	
	int colorType;	// 0 = nope, 1 = yup
	int colorValue;
	
	CSSConstant constant;	// 0 = nope, ....css_values.h
};

/*
// Used for multiple properties:
struct CSSLineStyle // CSS1 border-style, CSS2 outline-style, CSS3 column-rule-style
{ enum {
	NONE, // Default
	HIDDEN,
	DOTTED,
	DASHED,
	SOLID,
	DOUBLE,
	GROOVE,
	RIDGE,
	INSET,
	OUTSET,
}; };
struct CSSOverflow // CSS2 overflow, CSS3 overflow-x, CSS3 overflow-y
{ enum {
	VISIBLE, // Default
	HIDDEN,
	SCROLL,
	AUTO,
}; };
struct CSSBorderColor // CSS1 border-(bottom|top|left|right)-color
{ enum {
	TRANSPARENT, // Default
	VAL_COLOR=102,
}; };
struct CSSBorderWidth // CSS1 border-(bottom|top|left|right)-width
{ enum {
	VAL_LENGTH=101, // Default
}; };
/*struct CSSBorderRadius // CSS3 border-(bottom|top)-(left|right)-radius
{ enum {
	VAL_LENGTH=101, // Default
}; };*/


// Used for a single property:
/*struct CSSAlignContent // CSS3
{ enum {
	STRETCH, // Default
	CENTER,
	FLEX_START,
	FLEX_END,
	SPACE_BETWEEN,
	SPACE_AROUND,
}; };
struct CSSAlignItems // CSS3
{ enum {
	STRETCH, // Default
	CENTER,
	FLEX_START,
	FLEX_END,
	BASELINE,
}; };
struct CSSAlignSelf // CSS3
{ enum {
	AUTO, // Default
	STRETCH,
	CENTER,
	FLEX_START,
	FLEX_END,
	BASELINE,
}; };
struct CSSAnimationDirection // CSS3
{ enum {
	NORMAL, // Default
	REVERSE,
	ALTERNATE,
	ALTERNATE_REVERSE,
}; };
struct CSSAnimationFillMode // CSS3
{ enum {
	NONE, // Default
	FORWARDS,
	BACKWARDS,
	BOTH,
}; };
struct CSSAnimationPlayState // CSS3
{ enum {
	PAUSED,
	RUNNING, // Default
}; };
struct CSSBackfaceVisibility // CSS3
{ enum {
	VISIBLE, // Default
	HIDDEN,
}; };*//*
struct CSSBackgroundAttachment // CSS1
{ enum {
	SCROLL, // Default
	FIXED,
	LOCAL,
}; };
struct CSSBackgroundColor // CSS1
{ enum {
	TRANSPARENT, // Default
	VAL_COLOR=102,
}; };
struct CSSBackgroundImage // CSS1
{ enum {
	NONE, // Default
	VAL_URL=104,
}; };
/*struct CSSBackgroundClip // CSS3
{ enum {
	BORDER_BOX, // Default
	PADDING_BOX,
	CONTENT_BOX,
}; };*/

/*struct CSSBackgroundOrigin // CSS3
{ enum {
	PADDING_BOX, // Default
	BORDER_BOX,
	CONTENT_BOX,
}; };*//*
struct CSSBackgroundPositionX // CSS1
{ enum {
	LEFT,
	CENTER,
	RIGHT,
	VAL_LENGTH=101, // Default: 0%
}; };
struct CSSBackgroundPositionY // CSS1
{ enum {
	TOP,
	CENTER,
	BOTTOM,
	VAL_LENGTH=101, // Default: 0%
}; };

struct CSSBackgroundRepeat // CSS1
{ enum {
	REPEAT, // Default
	REPEAT_X,
	REPEAT_Y,
	NO_REPEAT,
}; };

/*struct CSSBackgroundSize // CSS3
{ enum {
	AUTO, // Default
	COVER,
	CONTAIN,
	VAL_LENGTH=101,
}; };*//*

struct CSSBorderCollapse // CSS2
{ enum {
	SEPARATE, // Default
	COLLAPSE,
}; };
struct CSSBorderSpacing // CSS2
{ enum {
	VAL_LENGTH=101, // Default: 0px
}; };
/*struct CSSBorderImageRepeat // CSS3
{ enum {
	STRETCH, // Default
	REPEAT,
	ROUND,
	SPACE,
}; };
struct CSSBoxSizing // CSS3
{ enum {
	CONTENT_BOX, // Default
	BORDER_BOX,
}; };*//*
struct CSSCaptionSide // CSS2
{ enum {
	TOP, // Default
	BOTTOM,
}; };
struct CSSClear // CSS1
{ enum {
	NONE, // Default
	LEFT,
	RIGHT,
	BOTH,
}; };
/*struct CSSColumnFill // CSS3
{ enum {
	BALANCE, // Default
	AUTO,
}; };
struct CSSColumnSpan // CSS3
{ enum {
	ONE, // Default
	ALL,
}; };*//*
struct CSSDirection // CSS2
{ enum {
	LTR, // Default
	RTL,
}; };
struct CSSDisplay
{ enum {
	INLINE,
	BLOCK,
	FLEX,
	INLINE_BLOCK,
	INLINE_FLEX,
	INLINE_TABLE,
	LIST_ITEM,
	RUN_IN,
	TABLE,
	TABLE_CAPTION,
	TABLE_COLUMN_GROUP,
	TABLE_HEADER_GROUP,
	TABLE_FOOTER_GROUP,
	TABLE_ROW_GROUP,
	TABLE_CELL,
	TABLE_COLUMN,
	TABLE_ROW,
	NONE,
}; };
struct CSSEmptyCells // CSS2
{ enum {
	SHOW, // Default
	HIDE,
}; };
/*struct CSSFlexDirection // CSS3
{ enum {
	ROW,
	ROW_REVERSE,
	COLUMN,
	COLUMN_REVERSE,
}; };
struct CSSFlexWrap // CSS3
{ enum {
	NOWRAP, // Default
	WRAP,
	WRAP_REVERSE,
}; };*//*
struct CSSFloat // CSS1
{ enum {
	NONE, // Default
	LEFT,
	RIGHT,
}; };
/*struct CSSFontStretch // CSS3
{ enum {
	ULTRA_CONDENSED,
	EXTRA_CONDENSED,
	CONDENSED,
	SEMI_CONDENSED,
	NORMAL, // Default
	SEMI_EXPANDED,
	EXPANDED,
	EXTRA_EXPANDED,
	ULTRA_EXPANDED,
}; };*//*
struct CSSFontStyle // CSS1
{ enum {
	NORMAL, // Default
	ITALIC,
	OBLIQUE,
}; };
struct CSSFontVariant // CSS1
{ enum {
	NORMAL, // Default
	SMALL_CAPS,
}; };
struct CSSFontWeight // CSS1
{ enum {
	ONE_HUNDRED,
	TWO_HUNDRED,
	THREE_HUNDRED,
	FOUR_HUNDRED,
	FIVE_HUNDRED,
	SIX_HUNDRED,
	SEVEN_HUNDRED,
	EIGHT_HUNDRED,
	NINE_HUNDRED,
	
	LIGHTER,
	NORMAL=ONE_HUNDRED, // Default
	BOLD=FOUR_HUNDRED,
	BOLDER=NINE_HUNDRED,
}; };
/*struct CSSHangingPunctuation // CSS3
{ enum {
	NONE, // Default
	FIRST,
	LAST,
	ALLOW_END,
	FORCE_END,
}; };
struct CSSJustifyContent // CSS3
{ enum {
	FLEX_START, // Default
	FLEX_END,
	CENTER,
	SPACE_BETWEEN,
	SPACE_AROUND,
}; };*//*
struct CSSListStylePosition // CSS1
{ enum {
	INSIDE,
	OUTSIDE, // Default
}; };
struct CSSListStyleType // CSS1
{ enum {
	DISC, // Default
	ARMENIAN,
	CIRCLE,
	CJK_IDEOGRAPHIC,
	DECIMAL,
	DECIMAL_LEADING_ZERO,
	GEORGIAN,
	HEBREW,
	HIRAGANA,
	HIRAGANA_IROHA,
	KATAKANA,
	KATAKANA_IROHA,
	LOWER_ALPHA,
	LOWER_GREEK,
	LOWER_LATIN,
	LOWER_ROMAN,
	NONE,
	SQUARE,
	UPPER_ALPHA,
	UPPER_LATIN,
	UPPER_ROMAN,
}; };
struct CSSPageBreakAfter // CSS2
{ enum {
	AUTO, // Default
	ALWAYS,
	AVOID,
	LEFT,
	RIGHT,
}; };
struct CSSPageBreakBefore // CSS2
{ enum {
	AUTO, // Default
	ALWAYS,
	AVOID,
	LEFT,
	RIGHT,
}; };
struct CSSPageBreakInside // CSS2
{ enum {
	AUTO, // Default
	AVOID,
	LEFT,
	RIGHT,
}; };
struct CSSPosition // CSS2
{ enum {
	STATIC, // Default
	ABSOLUTE,
	FIXED,
	RELATIVE,
}; };
/*
struct CSSResize // CSS3
{ enum {
	NONE, // Default
	BOTH,
	HORIZONTAL,
	VERTICAL,
}; };
*//*
struct CSSTableLayout // CSS2
{ enum {
	AUTO, // Default
	FIXED,
}; };
struct CSSTextAlign // CSS1
{ enum {
	LEFT, // Default for direction: ltr;
	RIGHT, // Default for direction: rtl;
	CENTER,
	JUSTIFY,
}; };
/*struct CSSTextAlignLast // CSS3
{ enum {
	AUTO, // Default
	LEFT,
	RIGHT,
	CENTER,
	JUSTIFY,
	START,
	END,
}; };
struct CSSTextDecorationLine // CSS3
{ enum {
	NONE, // Default
	UNDERLINE,
	OVERLINE,
	LINE_THROUGH,
}; };
struct CSSTextDecorationStyle // CSS3
{ enum {
	SOLID, // Default
	DOUBLE,
	DOTTED,
	DASHED,
	WAVY,
}; };
struct CSSTextJustify // CSS3
{ enum {
	AUTO,
	INTER_WORD,
	INTER_IDEOGRAPH,
	INTER_CLUSTER,
	DISTRIBUTE,
	KASHIDA,
	TRIM,
	NONE,
}; };*//*
struct CSSTextTransform // CSS1
{ enum {
	NONE,
	CAPITALIZE,
	UPPERCASE,
	LOWERCASE,
}; };
/*struct CSSTransformStyle // CSS3
{ enum {
	FLAT,
	PRESERVE_3D,
}; };*//*
struct CSSUnicodeBidi // CSS2
{ enum {
	NORMAL, // Default
	EMBED,
	BIDI_OVERRIDE,
}; };
struct CSSVisibility // CSS2
{ enum {
	VISIBLE, // Default
	HIDDEN,
	COLLAPSE,
}; };
struct CSSWhiteSpace // CSS1
{ enum {
	NORMAL, // Default
	NOWRAP,
	PRE,
	PRE_LINE,
	PRE_WRAP,
}; };
/*struct CSSWordBreak // CSS3
{ enum {
	NORMAL, // Default
	BREAK_ALL,
	KEEP_ALL,
}; };
struct CSSWordWrap // CSS3
{ enum {
	NORMAL, // DEFAULT
	BREAK_WORD,
}; };*/
#endif
