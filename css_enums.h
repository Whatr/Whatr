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

#ifndef css_enums_included
#define css_enums_included yes

// Used for multiple properties:
enum CSSLineStyle // CSS1 border-style, CSS2 outline-style, CSS3 column-rule-style
{
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
};
enum CSSOverflow // CSS2 overflow, CSS3 overflow-x, CSS3 overflow-y
{
	VISIBLE, // Default
	HIDDEN,
	SCROLL,
	AUTO,
}

// Used for a single property:
enum CSSAlignContent // CSS3
{
	STRETCH, // Default
	CENTER,
	FLEX_START,
	FLEX_END,
	SPACE_BETWEEN,
	SPACE_AROUND,
};
enum CSSAlignItems // CSS3
{
	STRETCH, // Default
	CENTER,
	FLEX_START,
	FLEX_END,
	BASELINE,
};
enum CSSAlignSelf // CSS3
{
	AUTO, // Default
	STRETCH,
	CENTER,
	FLEX_START,
	FLEX_END,
	BASELINE,
};
enum CSSAnimationDirection // CSS3
{
	NORMAL, // Default
	REVERSE,
	ALTERNATE,
	ALTERNATE_REVERSE,
};
enum CSSAnimationFillMode // CSS3
{
	NONE, // Default
	FORWARDS,
	BACKWARDS,
	BOTH,
};
enum CSSAnimationPlayState // CSS3
{
	PAUSED,
	RUNNING, // Default
};
enum CSSBackfaceVisibility // CSS3
{
	VISIBLE, // Default
	HIDDEN,
};
enum CSSBackgroundAttachment // CSS1
{
	SCROLL, // Default
	FIXED,
	LOCAL,
};
enum CSSBackgroundClip // CSS3
{
	BORDER_BOX, // Default
	PADDING_BOX,
	CONTENT_BOX,
};
enum CSSBackgroundOrigin // CSS3
{
	PADDING_BOX, // Default
	BORDER_BOX,
	CONTENT_BOX,
};
enum CSSBackgroundRepeat // CSS1
{
	REPEAT,
	REPEAT_X,
	REPEAT_Y,
	NO_REPEAT,
};
enum CSSBorderCollapse // CSS2
{
	SEPARATE, // Default
	COLLAPSE,
};
enum CSSBorderImageRepeat // CSS3
{
	STRETCH, // Default
	REPEAT,
	ROUND,
	SPACE,
};
enum CSSBoxSizing // CSS3
{
	CONTENT_BOX, // Default
	BORDER_BOX,
};
enum CSSCaptionSide // CSS2
{
	TOP, // Default
	BOTTOM,
};
enum CSSClear // CSS1
{
	NONE, // Default
	LEFT,
	RIGHT,
	BOTH,
};
enum CSSColumnFill // CSS3
{
	BALANCE, // Default
	AUTO,
};
enum CSSColumnSpan // CSS3
{
	ONE, // Default
	ALL,
};
enum CSSDirection // CSS2
{
	LTR, // Default
	RTL,
};
enum CSSDisplay
{
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
};
enum CSSEmptyCells // CSS2
{
	SHOW, // Default
	HIDE,
};
enum CSSFlexDirection // CSS3
{
	ROW,
	ROW_REVERSE,
	COLUMN,
	COLUMN_REVERSE,
};
enum CSSFlexWrap // CSS3
{
	NOWRAP, // Default
	WRAP,
	WRAP_REVERSE,
};
enum CSSFloat // CSS1
{
	NONE, // Default
	LEFT,
	RIGHT,
};
enum CSSFontStretch // CSS3
{
	ULTRA_CONDENSED,
	EXTRA_CONDENSED,
	CONDENSED,
	SEMI_CONDENSED,
	NORMAL, // Default
	SEMI_EXPANDED,
	EXPANDED,
	EXTRA_EXPANDED,
	ULTRA_EXPANDED,
};
enum CSSFontStyle // CSS1
{
	NORMAL, // Default
	ITALIC,
	OBLIQUE,
};
enum CSSFontVariant // CSS1
{
	NORMAL, // Default
	SMALL_CAPS,
};
enum CSSFontWeight // CSS1
{
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
};
enum CSSHangingPunctuation // CSS3
{
	NONE, // Default
	FIRST,
	LAST,
	ALLOW_END,
	FORCE_END,
};
enum CSSJustifyContent // CSS3
{
	FLEX_START, // Default
	FLEX_END,
	CENTER,
	SPACE_BETWEEN,
	SPACE_AROUND,
};
enum CSSListStylePosition // CSS1
{
	INSIDE,
	OUTSIDE, // Default
};
enum CSSListStyleType // CSS1
{
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
}
enum CSSPageBreakAfter // CSS2
{
	AUTO, // Default
	ALWAYS,
	AVOID,
	LEFT,
	RIGHT,
}
enum CSSPageBreakBefore // CSS2
{
	AUTO, // Default
	ALWAYS,
	AVOID,
	LEFT,
	RIGHT,
}
enum CSSPageBreakInside // CSS2
{
	AUTO, // Default
	AVOID,
	LEFT,
	RIGHT,
}
enum CSSPosition // CSS2
{
	STATIC, // Default
	ABSOLUTE,
	FIXED,
	RELATIVE,
};
enum CSSResize // CSS3
{
	NONE, // Default
	BOTH,
	HORIZONTAL,
	VERTICAL,
}
enum CSSTableLayout // CSS2
{
	AUTO, // Default
	FIXED,
}
enum CSSTextAlign // CSS1
{
	LEFT, // Default for direction: ltr;
	RIGHT, // Default for direction: rtl;
	CENTER,
	JUSTIFY,
}
enum CSSTextAlignLast // CSS3
{
	AUTO, // Default
	LEFT,
	RIGHT,
	CENTER,
	JUSTIFY,
	START,
	END,
}
enum CSSTextDecorationLine // CSS3
{
	NONE, // Default
	UNDERLINE,
	OVERLINE,
	LINE_THROUGH,
}
enum CSSTextDecorationStyle // CSS3
{
	SOLID, // Default
	DOUBLE,
	DOTTED,
	DASHED,
	WAVY,
}
enum CSSTextJustify // CSS3
{
	AUTO,
	INTER_WORD,
	INTER_IDEOGRAPH,
	INTER_CLUSTER,
	DISTRIBUTE,
	KASHIDA,
	TRIM,
	NONE,
}
enum CSSTextTransform // CSS1
{
	NONE,
	CAPITALIZE,
	UPPERCASE,
	LOWERCASE,
}
enum CSSTransformStyle // CSS3
{
	FLAT,
	PRESERVE_3D,
}
enum CSSUnicodeBidi // CSS2
{
	NORMAL, // Default
	EMBED,
	BIDI-OVERRIDE,
}
enum CSSVisibility // CSS2
{
	VISIBLE, // Default
	HIDDEN,
	COLLAPSE,
}
enum CSSWhiteSpace // CSS1
{
	NORMAL, // Default
	NOWRAP,
	PRE,
	PRE_LINE,
	PRE_WRAP,
}
enum CSSWordBreak // CSS3
{
	NORMAL, // Default
	BREAK_ALL,
	KEEP_ALL,
}
enum CSSWordWrap // CSS3
{
	NORMAL, // DEFAULT
	BREAK_WORD,
}
#endif
