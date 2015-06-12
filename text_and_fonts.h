#ifndef text_and_fonts_included
#define text_and_fonts_included yes

#include "const_str.h"

void calculateTextSize(ConstStr text, int availableWidth, int* charWidths, int* charHeights, int& widthOut, int& heightOut);
int loadFont(ConstStr name);

#endif
