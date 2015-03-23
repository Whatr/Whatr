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
enum CSSBorderStyle // CSS1
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
enum CSSColumnRuleStyle // CSS3
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
enum CSSPosition
{
	FIXED,
	ABSOLUTE,
	RELATIVE,
};
#endif
