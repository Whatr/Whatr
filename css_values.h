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

struct CSSValue
{
	int text;	// 0 = nope, 1 = yup
	int length;	// 0 = nope, 1 = px, 2 = em, 3 = %
	int color;	// 0 = nope, 1 = yup
	int time;	// 0 = nope, 1 = ms, 2 = s
	
	union
	{
		int textValue;
		double lengthValue;
		int colorValue;
		double timeValue;
	};
};

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
}; };*/
struct CSSBackgroundAttachment // CSS1
{ enum {
	SCROLL, // Default
	FIXED,
	LOCAL,
}; };
/*struct CSSBackgroundClip // CSS3
{ enum {
	BORDER_BOX, // Default
	PADDING_BOX,
	CONTENT_BOX,
}; };
struct CSSBackgroundOrigin // CSS3
{ enum {
	PADDING_BOX, // Default
	BORDER_BOX,
	CONTENT_BOX,
}; };*/
struct CSSBackgroundRepeat // CSS1
{ enum {
	REPEAT,
	REPEAT_X,
	REPEAT_Y,
	NO_REPEAT,
}; };
struct CSSBorderCollapse // CSS2
{ enum {
	SEPARATE, // Default
	COLLAPSE,
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
}; };*/
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
}; };*/
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
}; };*/
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
}; };*/
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
}; };*/
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
struct CSSResize // CSS3
{ enum {
	NONE, // Default
	BOTH,
	HORIZONTAL,
	VERTICAL,
}; };
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
}; };*/
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
}; };*/
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
