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

#ifndef css_properties_included
#define css_properties_included yes

enum CSSProperty
{
	// CSS1
	COLOR,
	OPACITY,
	
	BACKGROUND,
	BACKGROUND_ATTACHMENT,
	BACKGROUND_COLOR,
	BACKGROUND_IMAGE,
	BACKGROUND_POSITION,
	BACKROUND_REPEAT,
	
	BORDER_BOTTOM_COLOR,
	BORDER_BOTTOM_STYLE,
	BORDER_BOTTOM_WIDTH,
	BORDER_TOP_COLOR,
	BORDER_TOP_STYLE,
	BORDER_TOP_WIDTH,
	BORDER_LEFT_COLOR,
	BORDER_LEFT_STYLE,
	BORDER_LEFT_WIDTH,
	BORDER_RIGHT_COLOR,
	BORDER_RIGHT_STYLE,
	BORDER_RIGHT_WIDTH,
	
	CLEAR,
	DISPLAY,
	FLOAT,
	
	HEIGHT,
	WIDTH,
	
	PADDING_BOTTOM,
	PADDING_LEFT,
	PADDING_RIGHT,
	PADDING_TOP,
	
	VERTICAL_ALIGN,
	
	MARGIN_BOTTOM,
	MARGIN_LEFT,
	MARGIN_RIGHT,
	MARGIN_TOP,
	
	DISPLAY,
	
	LETTER_SPACING,
	LINE_HEIGHT,
	TEXT_ALIGN,
	TEXT_TRANSFORM,
	TEXT_INDENT,
	
	WHITE_SPACE,
	WORD_SPACING,
	TEXT_DECORATION,
	
	FONT_FAMILY,
	FONT_SIZE,
	FONT_STYLE,
	FONT_VARIANT,
	FONT_WEIGHT,
	
	LIST_STYLE_IMAGE,
	LIST_STYLE_POSITION,
	LIST_STYLE_TYPE,
	
	
	// CSS2
	/*
	CLIP,
	
	BOTTOM,
	TOP,
	LEFT,
	RIGHT,
	
	POSITION,
	VISIBILITY,
	Z_INDEX,
	
	MAX_HEIGHT,
	MAX_WIDTH,
	MIN_HEIGHT,
	MIN_WIDTH,
	
	// TODO: add the other CSS2 properties
	*/
	
	// CSS3
	/*
	BACKGROUND_CLIP,
	BACKGROUND_ORIGIN,
	BACKGROUND_SIZE,
	
	BORDER_BOTTOM_LEFT_RADIUS,
	BORDER_BOTTOM_RIGHT_RADIUS,
	BORDER_TOP_LEFT_RADIUS,
	BORDER_TOP_RIGHT_RADIUS,
	
	// TODO: add border image stuff
	
	BOX_DECORATION_BREAK,
	BOX_SHADOW,
	
	OVERFLOW,
	OVERFLOW_X,
	OVERFLOW_Y,
	
	ALIGN_CONTENT,
	ALIGN_ITEMS,
	ALIGN_SELF,
	
	FLEX,
	FLEX_BASIS,
	FLEX_DIRECTION,
	FLEX_FLOW,
	FLEX_GROW,
	FLEX_SHRINK,
	FLEX_WRAP,
	
	JUSTIFY_CONTENT,
	
	ORDER,
	
	HANGING_PUNCTUATION,
	HYPHENS,
	
	// TODO: add the other CSS3 properties
	
	*/
};

#endif
