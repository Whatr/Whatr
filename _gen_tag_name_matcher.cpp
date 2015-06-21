#include "html_tag_types.h"
#include "const_str.h"
HTMLTagType getTagType(ConstStr tag)
{
 int s = 0;
 ConstStrIterator i = tag.iterate();
 char c = *i;
 if (tag.length==1)
 {
    if (c == 'a') return TAG_A;
    else if (c == 'b') return TAG_B;
    else if (c == 'i') return TAG_I;
    else if (c == 'p') return TAG_P;
    else if (c == 'q') return TAG_Q;
    else if (c == 's') return TAG_S;
    else if (c == 'u') return TAG_U;
    return _TAG_CUSTOM;
 }
 if (tag.length==2)
 {
  for(;i<tag.length;i++)
  {
   c = *i;
   if (s==0)
   {
    if (c == 'b') s = 1;
    else if (c == 'd') s = 2;
    else if (c == 'e') s = 3;
    else if (c == 'h') s = 4;
    else if (c == 'l') s = 5;
    else if (c == 'o') s = 6;
    else if (c == 'r') s = 7;
    else if (c == 't') s = 8;
    else if (c == 'u') s = 9;
    else return _TAG_CUSTOM;
   }
   else    if (s==1)
   {
    if (c == 'r') return TAG_BR;
    else return _TAG_CUSTOM;
   }
   else if (s==2)
   {
    if (c == 'd') return TAG_DD;
    else if (c == 'l') return TAG_DL;
    else if (c == 't') return TAG_DT;
    else return _TAG_CUSTOM;
   }
   else if (s==3)
   {
    if (c == 'm') return TAG_EM;
    else return _TAG_CUSTOM;
   }
   else if (s==4)
   {
    if (c == '1') return TAG_H1;
    else if (c == '2') return TAG_H2;
    else if (c == '3') return TAG_H3;
    else if (c == '4') return TAG_H4;
    else if (c == '5') return TAG_H5;
    else if (c == '6') return TAG_H6;
    else if (c == 'r') return TAG_HR;
    else return _TAG_CUSTOM;
   }
   else if (s==5)
   {
    if (c == 'i') return TAG_LI;
    else return _TAG_CUSTOM;
   }
   else if (s==6)
   {
    if (c == 'l') return TAG_OL;
    else return _TAG_CUSTOM;
   }
   else if (s==7)
   {
    if (c == 'p') return TAG_RP;
    else if (c == 't') return TAG_RT;
    else return _TAG_CUSTOM;
   }
   else if (s==8)
   {
    if (c == 'd') return TAG_TD;
    else if (c == 'h') return TAG_TH;
    else if (c == 'r') return TAG_TR;
    else if (c == 't') return TAG_TT;
    else return _TAG_CUSTOM;
   }
   else if (s==9)
   {
    if (c == 'l') return TAG_UL;
    else return _TAG_CUSTOM;
   }
  }
 }
 if (tag.length==3)
 {
  for(;i<tag.length;i++)
  {
   c = *i;
   if (s==0)
   {
    if (c == 'b') s = 1;
    else if (c == 'c') s = 2;
    else if (c == 'd') s = 3;
    else if (c == 'i') s = 4;
    else if (c == 'k') s = 5;
    else if (c == 'm') s = 6;
    else if (c == 'n') s = 7;
    else if (c == 'p') s = 8;
    else if (c == 's') s = 9;
    else if (c == 'v') s = 10;
    else if (c == 'w') s = 11;
    else return _TAG_CUSTOM;
   }
   else    if (s==1)
   {
    if (c == 'd') s = 12;
    else if (c == 'i') s = 13;
    else return _TAG_CUSTOM;
   }
   else if (s==2)
   {
    if (c == 'o') s = 14;
    else return _TAG_CUSTOM;
   }
   else if (s==3)
   {
    if (c == 'e') s = 15;
    else if (c == 'f') s = 16;
    else if (c == 'i') s = 17;
    else return _TAG_CUSTOM;
   }
   else if (s==4)
   {
    if (c == 'm') s = 18;
    else if (c == 'n') s = 19;
    else return _TAG_CUSTOM;
   }
   else if (s==5)
   {
    if (c == 'b') s = 20;
    else return _TAG_CUSTOM;
   }
   else if (s==6)
   {
    if (c == 'a') s = 21;
    else return _TAG_CUSTOM;
   }
   else if (s==7)
   {
    if (c == 'a') s = 22;
    else return _TAG_CUSTOM;
   }
   else if (s==8)
   {
    if (c == 'r') s = 23;
    else return _TAG_CUSTOM;
   }
   else if (s==9)
   {
    if (c == 'u') s = 24;
    else return _TAG_CUSTOM;
   }
   else if (s==10)
   {
    if (c == 'a') s = 25;
    else return _TAG_CUSTOM;
   }
   else if (s==11)
   {
    if (c == 'b') s = 26;
    else return _TAG_CUSTOM;
   }
   else if (s==12)
   {
    if (c == 'i') return TAG_BDI;
    else if (c == 'o') return TAG_BDO;
    else return _TAG_CUSTOM;
   }
   else if (s==13)
   {
    if (c == 'g') return TAG_BIG;
    else return _TAG_CUSTOM;
   }
   else if (s==14)
   {
    if (c == 'l') return TAG_COL;
    else return _TAG_CUSTOM;
   }
   else if (s==15)
   {
    if (c == 'l') return TAG_DEL;
    else return _TAG_CUSTOM;
   }
   else if (s==16)
   {
    if (c == 'n') return TAG_DFN;
    else return _TAG_CUSTOM;
   }
   else if (s==17)
   {
    if (c == 'r') return TAG_DIR;
    else if (c == 'v') return TAG_DIV;
    else return _TAG_CUSTOM;
   }
   else if (s==18)
   {
    if (c == 'g') return TAG_IMG;
    else return _TAG_CUSTOM;
   }
   else if (s==19)
   {
    if (c == 's') return TAG_INS;
    else return _TAG_CUSTOM;
   }
   else if (s==20)
   {
    if (c == 'd') return TAG_KBD;
    else return _TAG_CUSTOM;
   }
   else if (s==21)
   {
    if (c == 'p') return TAG_MAP;
    else return _TAG_CUSTOM;
   }
   else if (s==22)
   {
    if (c == 'v') return TAG_NAV;
    else return _TAG_CUSTOM;
   }
   else if (s==23)
   {
    if (c == 'e') return TAG_PRE;
    else return _TAG_CUSTOM;
   }
   else if (s==24)
   {
    if (c == 'b') return TAG_SUB;
    else if (c == 'p') return TAG_SUP;
    else return _TAG_CUSTOM;
   }
   else if (s==25)
   {
    if (c == 'r') return TAG_VAR;
    else return _TAG_CUSTOM;
   }
   else if (s==26)
   {
    if (c == 'r') return TAG_WBR;
    else return _TAG_CUSTOM;
   }
  }
 }
 if (tag.length==4)
 {
  for(;i<tag.length;i++)
  {
   c = *i;
   if (s==0)
   {
    if (c == 'a') s = 1;
    else if (c == 'b') s = 2;
    else if (c == 'c') s = 3;
    else if (c == 'f') s = 4;
    else if (c == 'h') s = 5;
    else if (c == 'l') s = 6;
    else if (c == 'm') s = 7;
    else if (c == 'r') s = 8;
    else if (c == 's') s = 9;
    else if (c == 't') s = 10;
    else return _TAG_CUSTOM;
   }
   else    if (s==1)
   {
    if (c == 'b') s = 11;
    else if (c == 'r') s = 12;
    else return _TAG_CUSTOM;
   }
   else if (s==2)
   {
    if (c == 'a') s = 13;
    else if (c == 'o') s = 14;
    else return _TAG_CUSTOM;
   }
   else if (s==3)
   {
    if (c == 'i') s = 15;
    else if (c == 'o') s = 16;
    else return _TAG_CUSTOM;
   }
   else if (s==4)
   {
    if (c == 'o') s = 17;
    else return _TAG_CUSTOM;
   }
   else if (s==5)
   {
    if (c == 'e') s = 18;
    else if (c == 't') s = 19;
    else return _TAG_CUSTOM;
   }
   else if (s==6)
   {
    if (c == 'i') s = 20;
    else return _TAG_CUSTOM;
   }
   else if (s==7)
   {
    if (c == 'a') s = 21;
    else if (c == 'e') s = 22;
    else return _TAG_CUSTOM;
   }
   else if (s==8)
   {
    if (c == 'u') s = 23;
    else return _TAG_CUSTOM;
   }
   else if (s==9)
   {
    if (c == 'a') s = 24;
    else if (c == 'p') s = 25;
    else return _TAG_CUSTOM;
   }
   else if (s==10)
   {
    if (c == 'i') s = 26;
    else return _TAG_CUSTOM;
   }
   else if (s==11)
   {
    if (c == 'b') s = 27;
    else return _TAG_CUSTOM;
   }
   else if (s==12)
   {
    if (c == 'e') s = 28;
    else return _TAG_CUSTOM;
   }
   else if (s==13)
   {
    if (c == 's') s = 29;
    else return _TAG_CUSTOM;
   }
   else if (s==14)
   {
    if (c == 'd') s = 30;
    else return _TAG_CUSTOM;
   }
   else if (s==15)
   {
    if (c == 't') s = 31;
    else return _TAG_CUSTOM;
   }
   else if (s==16)
   {
    if (c == 'd') s = 32;
    else return _TAG_CUSTOM;
   }
   else if (s==17)
   {
    if (c == 'n') s = 33;
    else if (c == 'r') s = 34;
    else return _TAG_CUSTOM;
   }
   else if (s==18)
   {
    if (c == 'a') s = 35;
    else return _TAG_CUSTOM;
   }
   else if (s==19)
   {
    if (c == 'm') s = 36;
    else return _TAG_CUSTOM;
   }
   else if (s==20)
   {
    if (c == 'n') s = 37;
    else return _TAG_CUSTOM;
   }
   else if (s==21)
   {
    if (c == 'i') s = 38;
    else if (c == 'r') s = 39;
    else return _TAG_CUSTOM;
   }
   else if (s==22)
   {
    if (c == 'n') s = 40;
    else if (c == 't') s = 41;
    else return _TAG_CUSTOM;
   }
   else if (s==23)
   {
    if (c == 'b') s = 42;
    else return _TAG_CUSTOM;
   }
   else if (s==24)
   {
    if (c == 'm') s = 43;
    else return _TAG_CUSTOM;
   }
   else if (s==25)
   {
    if (c == 'a') s = 44;
    else return _TAG_CUSTOM;
   }
   else if (s==26)
   {
    if (c == 'm') s = 45;
    else return _TAG_CUSTOM;
   }
   else if (s==27)
   {
    if (c == 'r') return TAG_ABBR;
    else return _TAG_CUSTOM;
   }
   else if (s==28)
   {
    if (c == 'a') return TAG_AREA;
    else return _TAG_CUSTOM;
   }
   else if (s==29)
   {
    if (c == 'e') return TAG_BASE;
    else return _TAG_CUSTOM;
   }
   else if (s==30)
   {
    if (c == 'y') return TAG_BODY;
    else return _TAG_CUSTOM;
   }
   else if (s==31)
   {
    if (c == 'e') return TAG_CITE;
    else return _TAG_CUSTOM;
   }
   else if (s==32)
   {
    if (c == 'e') return TAG_CODE;
    else return _TAG_CUSTOM;
   }
   else if (s==33)
   {
    if (c == 't') return TAG_FONT;
    else return _TAG_CUSTOM;
   }
   else if (s==34)
   {
    if (c == 'm') return TAG_FORM;
    else return _TAG_CUSTOM;
   }
   else if (s==35)
   {
    if (c == 'd') return TAG_HEAD;
    else return _TAG_CUSTOM;
   }
   else if (s==36)
   {
    if (c == 'l') return TAG_HTML;
    else return _TAG_CUSTOM;
   }
   else if (s==37)
   {
    if (c == 'k') return TAG_LINK;
    else return _TAG_CUSTOM;
   }
   else if (s==38)
   {
    if (c == 'n') return TAG_MAIN;
    else return _TAG_CUSTOM;
   }
   else if (s==39)
   {
    if (c == 'k') return TAG_MARK;
    else return _TAG_CUSTOM;
   }
   else if (s==40)
   {
    if (c == 'u') return TAG_MENU;
    else return _TAG_CUSTOM;
   }
   else if (s==41)
   {
    if (c == 'a') return TAG_META;
    else return _TAG_CUSTOM;
   }
   else if (s==42)
   {
    if (c == 'y') return TAG_RUBY;
    else return _TAG_CUSTOM;
   }
   else if (s==43)
   {
    if (c == 'p') return TAG_SAMP;
    else return _TAG_CUSTOM;
   }
   else if (s==44)
   {
    if (c == 'n') return TAG_SPAN;
    else return _TAG_CUSTOM;
   }
   else if (s==45)
   {
    if (c == 'e') return TAG_TIME;
    else return _TAG_CUSTOM;
   }
  }
 }
 if (tag.length==5)
 {
  for(;i<tag.length;i++)
  {
   c = *i;
   if (s==0)
   {
    if (c == 'a') s = 1;
    else if (c == 'e') s = 2;
    else if (c == 'f') s = 3;
    else if (c == 'i') s = 4;
    else if (c == 'l') s = 5;
    else if (c == 'm') s = 6;
    else if (c == 'p') s = 7;
    else if (c == 's') s = 8;
    else if (c == 't') s = 9;
    else if (c == 'v') s = 10;
    else return _TAG_CUSTOM;
   }
   else    if (s==1)
   {
    if (c == 's') s = 11;
    else if (c == 'u') s = 12;
    else return _TAG_CUSTOM;
   }
   else if (s==2)
   {
    if (c == 'm') s = 13;
    else return _TAG_CUSTOM;
   }
   else if (s==3)
   {
    if (c == 'r') s = 14;
    else return _TAG_CUSTOM;
   }
   else if (s==4)
   {
    if (c == 'n') s = 15;
    else return _TAG_CUSTOM;
   }
   else if (s==5)
   {
    if (c == 'a') s = 16;
    else return _TAG_CUSTOM;
   }
   else if (s==6)
   {
    if (c == 'e') s = 17;
    else return _TAG_CUSTOM;
   }
   else if (s==7)
   {
    if (c == 'a') s = 18;
    else return _TAG_CUSTOM;
   }
   else if (s==8)
   {
    if (c == 'm') s = 19;
    else if (c == 't') s = 20;
    else return _TAG_CUSTOM;
   }
   else if (s==9)
   {
    if (c == 'a') s = 21;
    else if (c == 'b') s = 22;
    else if (c == 'f') s = 23;
    else if (c == 'h') s = 24;
    else if (c == 'i') s = 25;
    else if (c == 'r') s = 26;
    else return _TAG_CUSTOM;
   }
   else if (s==10)
   {
    if (c == 'i') s = 27;
    else return _TAG_CUSTOM;
   }
   else if (s==11)
   {
    if (c == 'i') s = 28;
    else return _TAG_CUSTOM;
   }
   else if (s==12)
   {
    if (c == 'd') s = 29;
    else return _TAG_CUSTOM;
   }
   else if (s==13)
   {
    if (c == 'b') s = 30;
    else return _TAG_CUSTOM;
   }
   else if (s==14)
   {
    if (c == 'a') s = 31;
    else return _TAG_CUSTOM;
   }
   else if (s==15)
   {
    if (c == 'p') s = 32;
    else return _TAG_CUSTOM;
   }
   else if (s==16)
   {
    if (c == 'b') s = 33;
    else return _TAG_CUSTOM;
   }
   else if (s==17)
   {
    if (c == 't') s = 34;
    else return _TAG_CUSTOM;
   }
   else if (s==18)
   {
    if (c == 'r') s = 35;
    else return _TAG_CUSTOM;
   }
   else if (s==19)
   {
    if (c == 'a') s = 36;
    else return _TAG_CUSTOM;
   }
   else if (s==20)
   {
    if (c == 'y') s = 37;
    else return _TAG_CUSTOM;
   }
   else if (s==21)
   {
    if (c == 'b') s = 38;
    else return _TAG_CUSTOM;
   }
   else if (s==22)
   {
    if (c == 'o') s = 39;
    else return _TAG_CUSTOM;
   }
   else if (s==23)
   {
    if (c == 'o') s = 40;
    else return _TAG_CUSTOM;
   }
   else if (s==24)
   {
    if (c == 'e') s = 41;
    else return _TAG_CUSTOM;
   }
   else if (s==25)
   {
    if (c == 't') s = 42;
    else return _TAG_CUSTOM;
   }
   else if (s==26)
   {
    if (c == 'a') s = 43;
    else return _TAG_CUSTOM;
   }
   else if (s==27)
   {
    if (c == 'd') s = 44;
    else return _TAG_CUSTOM;
   }
   else if (s==28)
   {
    if (c == 'd') s = 45;
    else return _TAG_CUSTOM;
   }
   else if (s==29)
   {
    if (c == 'i') s = 46;
    else return _TAG_CUSTOM;
   }
   else if (s==30)
   {
    if (c == 'e') s = 47;
    else return _TAG_CUSTOM;
   }
   else if (s==31)
   {
    if (c == 'm') s = 48;
    else return _TAG_CUSTOM;
   }
   else if (s==32)
   {
    if (c == 'u') s = 49;
    else return _TAG_CUSTOM;
   }
   else if (s==33)
   {
    if (c == 'e') s = 50;
    else return _TAG_CUSTOM;
   }
   else if (s==34)
   {
    if (c == 'e') s = 51;
    else return _TAG_CUSTOM;
   }
   else if (s==35)
   {
    if (c == 'a') s = 52;
    else return _TAG_CUSTOM;
   }
   else if (s==36)
   {
    if (c == 'l') s = 53;
    else return _TAG_CUSTOM;
   }
   else if (s==37)
   {
    if (c == 'l') s = 54;
    else return _TAG_CUSTOM;
   }
   else if (s==38)
   {
    if (c == 'l') s = 55;
    else return _TAG_CUSTOM;
   }
   else if (s==39)
   {
    if (c == 'd') s = 56;
    else return _TAG_CUSTOM;
   }
   else if (s==40)
   {
    if (c == 'o') s = 57;
    else return _TAG_CUSTOM;
   }
   else if (s==41)
   {
    if (c == 'a') s = 58;
    else return _TAG_CUSTOM;
   }
   else if (s==42)
   {
    if (c == 'l') s = 59;
    else return _TAG_CUSTOM;
   }
   else if (s==43)
   {
    if (c == 'c') s = 60;
    else return _TAG_CUSTOM;
   }
   else if (s==44)
   {
    if (c == 'e') s = 61;
    else return _TAG_CUSTOM;
   }
   else if (s==45)
   {
    if (c == 'e') return TAG_ASIDE;
    else return _TAG_CUSTOM;
   }
   else if (s==46)
   {
    if (c == 'o') return TAG_AUDIO;
    else return _TAG_CUSTOM;
   }
   else if (s==47)
   {
    if (c == 'd') return TAG_EMBED;
    else return _TAG_CUSTOM;
   }
   else if (s==48)
   {
    if (c == 'e') return TAG_FRAME;
    else return _TAG_CUSTOM;
   }
   else if (s==49)
   {
    if (c == 't') return TAG_INPUT;
    else return _TAG_CUSTOM;
   }
   else if (s==50)
   {
    if (c == 'l') return TAG_LABEL;
    else return _TAG_CUSTOM;
   }
   else if (s==51)
   {
    if (c == 'r') return TAG_METER;
    else return _TAG_CUSTOM;
   }
   else if (s==52)
   {
    if (c == 'm') return TAG_PARAM;
    else return _TAG_CUSTOM;
   }
   else if (s==53)
   {
    if (c == 'l') return TAG_SMALL;
    else return _TAG_CUSTOM;
   }
   else if (s==54)
   {
    if (c == 'e') return TAG_STYLE;
    else return _TAG_CUSTOM;
   }
   else if (s==55)
   {
    if (c == 'e') return TAG_TABLE;
    else return _TAG_CUSTOM;
   }
   else if (s==56)
   {
    if (c == 'y') return TAG_TBODY;
    else return _TAG_CUSTOM;
   }
   else if (s==57)
   {
    if (c == 't') return TAG_TFOOT;
    else return _TAG_CUSTOM;
   }
   else if (s==58)
   {
    if (c == 'd') return TAG_THEAD;
    else return _TAG_CUSTOM;
   }
   else if (s==59)
   {
    if (c == 'e') return TAG_TITLE;
    else return _TAG_CUSTOM;
   }
   else if (s==60)
   {
    if (c == 'k') return TAG_TRACK;
    else return _TAG_CUSTOM;
   }
   else if (s==61)
   {
    if (c == 'o') return TAG_VIDEO;
    else return _TAG_CUSTOM;
   }
  }
 }
 if (tag.length==6)
 {
  for(;i<tag.length;i++)
  {
   c = *i;
   if (s==0)
   {
    if (c == 'a') s = 1;
    else if (c == 'b') s = 2;
    else if (c == 'c') s = 3;
    else if (c == 'd') s = 4;
    else if (c == 'f') s = 5;
    else if (c == 'h') s = 6;
    else if (c == 'i') s = 7;
    else if (c == 'k') s = 8;
    else if (c == 'l') s = 9;
    else if (c == 'o') s = 10;
    else if (c == 's') s = 11;
    else return _TAG_CUSTOM;
   }
   else    if (s==1)
   {
    if (c == 'p') s = 12;
    else return _TAG_CUSTOM;
   }
   else if (s==2)
   {
    if (c == 'u') s = 13;
    else return _TAG_CUSTOM;
   }
   else if (s==3)
   {
    if (c == 'a') s = 14;
    else if (c == 'e') s = 15;
    else return _TAG_CUSTOM;
   }
   else if (s==4)
   {
    if (c == 'i') s = 16;
    else return _TAG_CUSTOM;
   }
   else if (s==5)
   {
    if (c == 'i') s = 17;
    else if (c == 'o') s = 18;
    else return _TAG_CUSTOM;
   }
   else if (s==6)
   {
    if (c == 'e') s = 19;
    else return _TAG_CUSTOM;
   }
   else if (s==7)
   {
    if (c == 'f') s = 20;
    else return _TAG_CUSTOM;
   }
   else if (s==8)
   {
    if (c == 'e') s = 21;
    else return _TAG_CUSTOM;
   }
   else if (s==9)
   {
    if (c == 'e') s = 22;
    else return _TAG_CUSTOM;
   }
   else if (s==10)
   {
    if (c == 'b') s = 23;
    else if (c == 'p') s = 24;
    else if (c == 'u') s = 25;
    else return _TAG_CUSTOM;
   }
   else if (s==11)
   {
    if (c == 'c') s = 26;
    else if (c == 'e') s = 27;
    else if (c == 'o') s = 28;
    else if (c == 't') s = 29;
    else return _TAG_CUSTOM;
   }
   else if (s==12)
   {
    if (c == 'p') s = 30;
    else return _TAG_CUSTOM;
   }
   else if (s==13)
   {
    if (c == 't') s = 31;
    else return _TAG_CUSTOM;
   }
   else if (s==14)
   {
    if (c == 'n') s = 32;
    else return _TAG_CUSTOM;
   }
   else if (s==15)
   {
    if (c == 'n') s = 33;
    else return _TAG_CUSTOM;
   }
   else if (s==16)
   {
    if (c == 'a') s = 34;
    else return _TAG_CUSTOM;
   }
   else if (s==17)
   {
    if (c == 'g') s = 35;
    else return _TAG_CUSTOM;
   }
   else if (s==18)
   {
    if (c == 'o') s = 36;
    else return _TAG_CUSTOM;
   }
   else if (s==19)
   {
    if (c == 'a') s = 37;
    else return _TAG_CUSTOM;
   }
   else if (s==20)
   {
    if (c == 'r') s = 38;
    else return _TAG_CUSTOM;
   }
   else if (s==21)
   {
    if (c == 'y') s = 39;
    else return _TAG_CUSTOM;
   }
   else if (s==22)
   {
    if (c == 'g') s = 40;
    else return _TAG_CUSTOM;
   }
   else if (s==23)
   {
    if (c == 'j') s = 41;
    else return _TAG_CUSTOM;
   }
   else if (s==24)
   {
    if (c == 't') s = 42;
    else return _TAG_CUSTOM;
   }
   else if (s==25)
   {
    if (c == 't') s = 43;
    else return _TAG_CUSTOM;
   }
   else if (s==26)
   {
    if (c == 'r') s = 44;
    else return _TAG_CUSTOM;
   }
   else if (s==27)
   {
    if (c == 'l') s = 45;
    else return _TAG_CUSTOM;
   }
   else if (s==28)
   {
    if (c == 'u') s = 46;
    else return _TAG_CUSTOM;
   }
   else if (s==29)
   {
    if (c == 'r') s = 47;
    else return _TAG_CUSTOM;
   }
   else if (s==30)
   {
    if (c == 'l') s = 48;
    else return _TAG_CUSTOM;
   }
   else if (s==31)
   {
    if (c == 't') s = 49;
    else return _TAG_CUSTOM;
   }
   else if (s==32)
   {
    if (c == 'v') s = 50;
    else return _TAG_CUSTOM;
   }
   else if (s==33)
   {
    if (c == 't') s = 51;
    else return _TAG_CUSTOM;
   }
   else if (s==34)
   {
    if (c == 'l') s = 52;
    else return _TAG_CUSTOM;
   }
   else if (s==35)
   {
    if (c == 'u') s = 53;
    else return _TAG_CUSTOM;
   }
   else if (s==36)
   {
    if (c == 't') s = 54;
    else return _TAG_CUSTOM;
   }
   else if (s==37)
   {
    if (c == 'd') s = 55;
    else return _TAG_CUSTOM;
   }
   else if (s==38)
   {
    if (c == 'a') s = 56;
    else return _TAG_CUSTOM;
   }
   else if (s==39)
   {
    if (c == 'g') s = 57;
    else return _TAG_CUSTOM;
   }
   else if (s==40)
   {
    if (c == 'e') s = 58;
    else return _TAG_CUSTOM;
   }
   else if (s==41)
   {
    if (c == 'e') s = 59;
    else return _TAG_CUSTOM;
   }
   else if (s==42)
   {
    if (c == 'i') s = 60;
    else return _TAG_CUSTOM;
   }
   else if (s==43)
   {
    if (c == 'p') s = 61;
    else return _TAG_CUSTOM;
   }
   else if (s==44)
   {
    if (c == 'i') s = 62;
    else return _TAG_CUSTOM;
   }
   else if (s==45)
   {
    if (c == 'e') s = 63;
    else return _TAG_CUSTOM;
   }
   else if (s==46)
   {
    if (c == 'r') s = 64;
    else return _TAG_CUSTOM;
   }
   else if (s==47)
   {
    if (c == 'i') s = 65;
    else if (c == 'o') s = 66;
    else return _TAG_CUSTOM;
   }
   else if (s==48)
   {
    if (c == 'e') s = 67;
    else return _TAG_CUSTOM;
   }
   else if (s==49)
   {
    if (c == 'o') s = 68;
    else return _TAG_CUSTOM;
   }
   else if (s==50)
   {
    if (c == 'a') s = 69;
    else return _TAG_CUSTOM;
   }
   else if (s==51)
   {
    if (c == 'e') s = 70;
    else return _TAG_CUSTOM;
   }
   else if (s==52)
   {
    if (c == 'o') s = 71;
    else return _TAG_CUSTOM;
   }
   else if (s==53)
   {
    if (c == 'r') s = 72;
    else return _TAG_CUSTOM;
   }
   else if (s==54)
   {
    if (c == 'e') s = 73;
    else return _TAG_CUSTOM;
   }
   else if (s==55)
   {
    if (c == 'e') s = 74;
    else return _TAG_CUSTOM;
   }
   else if (s==56)
   {
    if (c == 'm') s = 75;
    else return _TAG_CUSTOM;
   }
   else if (s==57)
   {
    if (c == 'e') s = 76;
    else return _TAG_CUSTOM;
   }
   else if (s==58)
   {
    if (c == 'n') s = 77;
    else return _TAG_CUSTOM;
   }
   else if (s==59)
   {
    if (c == 'c') s = 78;
    else return _TAG_CUSTOM;
   }
   else if (s==60)
   {
    if (c == 'o') s = 79;
    else return _TAG_CUSTOM;
   }
   else if (s==61)
   {
    if (c == 'u') s = 80;
    else return _TAG_CUSTOM;
   }
   else if (s==62)
   {
    if (c == 'p') s = 81;
    else return _TAG_CUSTOM;
   }
   else if (s==63)
   {
    if (c == 'c') s = 82;
    else return _TAG_CUSTOM;
   }
   else if (s==64)
   {
    if (c == 'c') s = 83;
    else return _TAG_CUSTOM;
   }
   else if (s==65)
   {
    if (c == 'k') s = 84;
    else return _TAG_CUSTOM;
   }
   else if (s==66)
   {
    if (c == 'n') s = 85;
    else return _TAG_CUSTOM;
   }
   else if (s==67)
   {
    if (c == 't') return TAG_APPLET;
    else return _TAG_CUSTOM;
   }
   else if (s==68)
   {
    if (c == 'n') return TAG_BUTTON;
    else return _TAG_CUSTOM;
   }
   else if (s==69)
   {
    if (c == 's') return TAG_CANVAS;
    else return _TAG_CUSTOM;
   }
   else if (s==70)
   {
    if (c == 'r') return TAG_CENTER;
    else return _TAG_CUSTOM;
   }
   else if (s==71)
   {
    if (c == 'g') return TAG_DIALOG;
    else return _TAG_CUSTOM;
   }
   else if (s==72)
   {
    if (c == 'e') return TAG_FIGURE;
    else return _TAG_CUSTOM;
   }
   else if (s==73)
   {
    if (c == 'r') return TAG_FOOTER;
    else return _TAG_CUSTOM;
   }
   else if (s==74)
   {
    if (c == 'r') return TAG_HEADER;
    else return _TAG_CUSTOM;
   }
   else if (s==75)
   {
    if (c == 'e') return TAG_IFRAME;
    else return _TAG_CUSTOM;
   }
   else if (s==76)
   {
    if (c == 'n') return TAG_KEYGEN;
    else return _TAG_CUSTOM;
   }
   else if (s==77)
   {
    if (c == 'd') return TAG_LEGEND;
    else return _TAG_CUSTOM;
   }
   else if (s==78)
   {
    if (c == 't') return TAG_OBJECT;
    else return _TAG_CUSTOM;
   }
   else if (s==79)
   {
    if (c == 'n') return TAG_OPTION;
    else return _TAG_CUSTOM;
   }
   else if (s==80)
   {
    if (c == 't') return TAG_OUTPUT;
    else return _TAG_CUSTOM;
   }
   else if (s==81)
   {
    if (c == 't') return TAG_SCRIPT;
    else return _TAG_CUSTOM;
   }
   else if (s==82)
   {
    if (c == 't') return TAG_SELECT;
    else return _TAG_CUSTOM;
   }
   else if (s==83)
   {
    if (c == 'e') return TAG_SOURCE;
    else return _TAG_CUSTOM;
   }
   else if (s==84)
   {
    if (c == 'e') return TAG_STRIKE;
    else return _TAG_CUSTOM;
   }
   else if (s==85)
   {
    if (c == 'g') return TAG_STRONG;
    else return _TAG_CUSTOM;
   }
  }
 }
 if (tag.length==7)
 {
  for(;i<tag.length;i++)
  {
   c = *i;
   if (s==0)
   {
    if (c == 'a') s = 1;
    else if (c == 'c') s = 2;
    else if (c == 'd') s = 3;
    else if (c == 's') s = 4;
    else return _TAG_CUSTOM;
   }
   else    if (s==1)
   {
    if (c == 'c') s = 5;
    else if (c == 'd') s = 6;
    else if (c == 'r') s = 7;
    else return _TAG_CUSTOM;
   }
   else if (s==2)
   {
    if (c == 'a') s = 8;
    else return _TAG_CUSTOM;
   }
   else if (s==3)
   {
    if (c == 'e') s = 9;
    else return _TAG_CUSTOM;
   }
   else if (s==4)
   {
    if (c == 'e') s = 10;
    else if (c == 'u') s = 11;
    else return _TAG_CUSTOM;
   }
   else if (s==5)
   {
    if (c == 'r') s = 12;
    else return _TAG_CUSTOM;
   }
   else if (s==6)
   {
    if (c == 'd') s = 13;
    else return _TAG_CUSTOM;
   }
   else if (s==7)
   {
    if (c == 't') s = 14;
    else return _TAG_CUSTOM;
   }
   else if (s==8)
   {
    if (c == 'p') s = 15;
    else return _TAG_CUSTOM;
   }
   else if (s==9)
   {
    if (c == 't') s = 16;
    else return _TAG_CUSTOM;
   }
   else if (s==10)
   {
    if (c == 'c') s = 17;
    else return _TAG_CUSTOM;
   }
   else if (s==11)
   {
    if (c == 'm') s = 18;
    else return _TAG_CUSTOM;
   }
   else if (s==12)
   {
    if (c == 'o') s = 19;
    else return _TAG_CUSTOM;
   }
   else if (s==13)
   {
    if (c == 'r') s = 20;
    else return _TAG_CUSTOM;
   }
   else if (s==14)
   {
    if (c == 'i') s = 21;
    else return _TAG_CUSTOM;
   }
   else if (s==15)
   {
    if (c == 't') s = 22;
    else return _TAG_CUSTOM;
   }
   else if (s==16)
   {
    if (c == 'a') s = 23;
    else return _TAG_CUSTOM;
   }
   else if (s==17)
   {
    if (c == 't') s = 24;
    else return _TAG_CUSTOM;
   }
   else if (s==18)
   {
    if (c == 'm') s = 25;
    else return _TAG_CUSTOM;
   }
   else if (s==19)
   {
    if (c == 'n') s = 26;
    else return _TAG_CUSTOM;
   }
   else if (s==20)
   {
    if (c == 'e') s = 27;
    else return _TAG_CUSTOM;
   }
   else if (s==21)
   {
    if (c == 'c') s = 28;
    else return _TAG_CUSTOM;
   }
   else if (s==22)
   {
    if (c == 'i') s = 29;
    else return _TAG_CUSTOM;
   }
   else if (s==23)
   {
    if (c == 'i') s = 30;
    else return _TAG_CUSTOM;
   }
   else if (s==24)
   {
    if (c == 'i') s = 31;
    else return _TAG_CUSTOM;
   }
   else if (s==25)
   {
    if (c == 'a') s = 32;
    else return _TAG_CUSTOM;
   }
   else if (s==26)
   {
    if (c == 'y') s = 33;
    else return _TAG_CUSTOM;
   }
   else if (s==27)
   {
    if (c == 's') s = 34;
    else return _TAG_CUSTOM;
   }
   else if (s==28)
   {
    if (c == 'l') s = 35;
    else return _TAG_CUSTOM;
   }
   else if (s==29)
   {
    if (c == 'o') s = 36;
    else return _TAG_CUSTOM;
   }
   else if (s==30)
   {
    if (c == 'l') s = 37;
    else return _TAG_CUSTOM;
   }
   else if (s==31)
   {
    if (c == 'o') s = 38;
    else return _TAG_CUSTOM;
   }
   else if (s==32)
   {
    if (c == 'r') s = 39;
    else return _TAG_CUSTOM;
   }
   else if (s==33)
   {
    if (c == 'm') return TAG_ACRONYM;
    else return _TAG_CUSTOM;
   }
   else if (s==34)
   {
    if (c == 's') return TAG_ADDRESS;
    else return _TAG_CUSTOM;
   }
   else if (s==35)
   {
    if (c == 'e') return TAG_ARTICLE;
    else return _TAG_CUSTOM;
   }
   else if (s==36)
   {
    if (c == 'n') return TAG_CAPTION;
    else return _TAG_CUSTOM;
   }
   else if (s==37)
   {
    if (c == 's') return TAG_DETAILS;
    else return _TAG_CUSTOM;
   }
   else if (s==38)
   {
    if (c == 'n') return TAG_SECTION;
    else return _TAG_CUSTOM;
   }
   else if (s==39)
   {
    if (c == 'y') return TAG_SUMMARY;
    else return _TAG_CUSTOM;
   }
  }
 }
 if (tag.length==8)
 {
  for(;i<tag.length;i++)
  {
   c = *i;
   if (s==0)
   {
    if (c == '!') s = 1;
    else if (c == 'b') s = 2;
    else if (c == 'c') s = 3;
    else if (c == 'd') s = 4;
    else if (c == 'f') s = 5;
    else if (c == 'm') s = 6;
    else if (c == 'n') s = 7;
    else if (c == 'o') s = 8;
    else if (c == 'p') s = 9;
    else if (c == 't') s = 10;
    else return _TAG_CUSTOM;
   }
   else    if (s==1)
   {
    if (c == 'd') s = 11;
    else return _TAG_CUSTOM;
   }
   else if (s==2)
   {
    if (c == 'a') s = 12;
    else return _TAG_CUSTOM;
   }
   else if (s==3)
   {
    if (c == 'o') s = 13;
    else return _TAG_CUSTOM;
   }
   else if (s==4)
   {
    if (c == 'a') s = 14;
    else return _TAG_CUSTOM;
   }
   else if (s==5)
   {
    if (c == 'i') s = 15;
    else if (c == 'r') s = 16;
    else return _TAG_CUSTOM;
   }
   else if (s==6)
   {
    if (c == 'e') s = 17;
    else return _TAG_CUSTOM;
   }
   else if (s==7)
   {
    if (c == 'o') s = 18;
    else return _TAG_CUSTOM;
   }
   else if (s==8)
   {
    if (c == 'p') s = 19;
    else return _TAG_CUSTOM;
   }
   else if (s==9)
   {
    if (c == 'r') s = 20;
    else return _TAG_CUSTOM;
   }
   else if (s==10)
   {
    if (c == 'e') s = 21;
    else return _TAG_CUSTOM;
   }
   else if (s==11)
   {
    if (c == 'o') s = 22;
    else return _TAG_CUSTOM;
   }
   else if (s==12)
   {
    if (c == 's') s = 23;
    else return _TAG_CUSTOM;
   }
   else if (s==13)
   {
    if (c == 'l') s = 24;
    else return _TAG_CUSTOM;
   }
   else if (s==14)
   {
    if (c == 't') s = 25;
    else return _TAG_CUSTOM;
   }
   else if (s==15)
   {
    if (c == 'e') s = 26;
    else return _TAG_CUSTOM;
   }
   else if (s==16)
   {
    if (c == 'a') s = 27;
    else return _TAG_CUSTOM;
   }
   else if (s==17)
   {
    if (c == 'n') s = 28;
    else return _TAG_CUSTOM;
   }
   else if (s==18)
   {
    if (c == 'f') s = 29;
    else if (c == 's') s = 30;
    else return _TAG_CUSTOM;
   }
   else if (s==19)
   {
    if (c == 't') s = 31;
    else return _TAG_CUSTOM;
   }
   else if (s==20)
   {
    if (c == 'o') s = 32;
    else return _TAG_CUSTOM;
   }
   else if (s==21)
   {
    if (c == 'x') s = 33;
    else return _TAG_CUSTOM;
   }
   else if (s==22)
   {
    if (c == 'c') s = 34;
    else return _TAG_CUSTOM;
   }
   else if (s==23)
   {
    if (c == 'e') s = 35;
    else return _TAG_CUSTOM;
   }
   else if (s==24)
   {
    if (c == 'g') s = 36;
    else return _TAG_CUSTOM;
   }
   else if (s==25)
   {
    if (c == 'a') s = 37;
    else return _TAG_CUSTOM;
   }
   else if (s==26)
   {
    if (c == 'l') s = 38;
    else return _TAG_CUSTOM;
   }
   else if (s==27)
   {
    if (c == 'm') s = 39;
    else return _TAG_CUSTOM;
   }
   else if (s==28)
   {
    if (c == 'u') s = 40;
    else return _TAG_CUSTOM;
   }
   else if (s==29)
   {
    if (c == 'r') s = 41;
    else return _TAG_CUSTOM;
   }
   else if (s==30)
   {
    if (c == 'c') s = 42;
    else return _TAG_CUSTOM;
   }
   else if (s==31)
   {
    if (c == 'g') s = 43;
    else return _TAG_CUSTOM;
   }
   else if (s==32)
   {
    if (c == 'g') s = 44;
    else return _TAG_CUSTOM;
   }
   else if (s==33)
   {
    if (c == 't') s = 45;
    else return _TAG_CUSTOM;
   }
   else if (s==34)
   {
    if (c == 't') s = 46;
    else return _TAG_CUSTOM;
   }
   else if (s==35)
   {
    if (c == 'f') s = 47;
    else return _TAG_CUSTOM;
   }
   else if (s==36)
   {
    if (c == 'r') s = 48;
    else return _TAG_CUSTOM;
   }
   else if (s==37)
   {
    if (c == 'l') s = 49;
    else return _TAG_CUSTOM;
   }
   else if (s==38)
   {
    if (c == 'd') s = 50;
    else return _TAG_CUSTOM;
   }
   else if (s==39)
   {
    if (c == 'e') s = 51;
    else return _TAG_CUSTOM;
   }
   else if (s==40)
   {
    if (c == 'i') s = 52;
    else return _TAG_CUSTOM;
   }
   else if (s==41)
   {
    if (c == 'a') s = 53;
    else return _TAG_CUSTOM;
   }
   else if (s==42)
   {
    if (c == 'r') s = 54;
    else return _TAG_CUSTOM;
   }
   else if (s==43)
   {
    if (c == 'r') s = 55;
    else return _TAG_CUSTOM;
   }
   else if (s==44)
   {
    if (c == 'r') s = 56;
    else return _TAG_CUSTOM;
   }
   else if (s==45)
   {
    if (c == 'a') s = 57;
    else return _TAG_CUSTOM;
   }
   else if (s==46)
   {
    if (c == 'y') s = 58;
    else return _TAG_CUSTOM;
   }
   else if (s==47)
   {
    if (c == 'o') s = 59;
    else return _TAG_CUSTOM;
   }
   else if (s==48)
   {
    if (c == 'o') s = 60;
    else return _TAG_CUSTOM;
   }
   else if (s==49)
   {
    if (c == 'i') s = 61;
    else return _TAG_CUSTOM;
   }
   else if (s==50)
   {
    if (c == 's') s = 62;
    else return _TAG_CUSTOM;
   }
   else if (s==51)
   {
    if (c == 's') s = 63;
    else return _TAG_CUSTOM;
   }
   else if (s==52)
   {
    if (c == 't') s = 64;
    else return _TAG_CUSTOM;
   }
   else if (s==53)
   {
    if (c == 'm') s = 65;
    else return _TAG_CUSTOM;
   }
   else if (s==54)
   {
    if (c == 'i') s = 66;
    else return _TAG_CUSTOM;
   }
   else if (s==55)
   {
    if (c == 'o') s = 67;
    else return _TAG_CUSTOM;
   }
   else if (s==56)
   {
    if (c == 'e') s = 68;
    else return _TAG_CUSTOM;
   }
   else if (s==57)
   {
    if (c == 'r') s = 69;
    else return _TAG_CUSTOM;
   }
   else if (s==58)
   {
    if (c == 'p') s = 70;
    else return _TAG_CUSTOM;
   }
   else if (s==59)
   {
    if (c == 'n') s = 71;
    else return _TAG_CUSTOM;
   }
   else if (s==60)
   {
    if (c == 'u') s = 72;
    else return _TAG_CUSTOM;
   }
   else if (s==61)
   {
    if (c == 's') s = 73;
    else return _TAG_CUSTOM;
   }
   else if (s==62)
   {
    if (c == 'e') s = 74;
    else return _TAG_CUSTOM;
   }
   else if (s==63)
   {
    if (c == 'e') s = 75;
    else return _TAG_CUSTOM;
   }
   else if (s==64)
   {
    if (c == 'e') s = 76;
    else return _TAG_CUSTOM;
   }
   else if (s==65)
   {
    if (c == 'e') s = 77;
    else return _TAG_CUSTOM;
   }
   else if (s==66)
   {
    if (c == 'p') s = 78;
    else return _TAG_CUSTOM;
   }
   else if (s==67)
   {
    if (c == 'u') s = 79;
    else return _TAG_CUSTOM;
   }
   else if (s==68)
   {
    if (c == 's') s = 80;
    else return _TAG_CUSTOM;
   }
   else if (s==69)
   {
    if (c == 'e') s = 81;
    else return _TAG_CUSTOM;
   }
   else if (s==70)
   {
    if (c == 'e') return TAG_DOCTYPE;
    else return _TAG_CUSTOM;
   }
   else if (s==71)
   {
    if (c == 't') return TAG_BASEFONT;
    else return _TAG_CUSTOM;
   }
   else if (s==72)
   {
    if (c == 'p') return TAG_COLGROUP;
    else return _TAG_CUSTOM;
   }
   else if (s==73)
   {
    if (c == 't') return TAG_DATALIST;
    else return _TAG_CUSTOM;
   }
   else if (s==74)
   {
    if (c == 't') return TAG_FIELDSET;
    else return _TAG_CUSTOM;
   }
   else if (s==75)
   {
    if (c == 't') return TAG_FRAMESET;
    else return _TAG_CUSTOM;
   }
   else if (s==76)
   {
    if (c == 'm') return TAG_MENUITEM;
    else return _TAG_CUSTOM;
   }
   else if (s==77)
   {
    if (c == 's') return TAG_NOFRAMES;
    else return _TAG_CUSTOM;
   }
   else if (s==78)
   {
    if (c == 't') return TAG_NOSCRIPT;
    else return _TAG_CUSTOM;
   }
   else if (s==79)
   {
    if (c == 'p') return TAG_OPTGROUP;
    else return _TAG_CUSTOM;
   }
   else if (s==80)
   {
    if (c == 's') return TAG_PROGRESS;
    else return _TAG_CUSTOM;
   }
   else if (s==81)
   {
    if (c == 'a') return TAG_TEXTAREA;
    else return _TAG_CUSTOM;
   }
  }
 }
 if (tag.length==10)
 {
  for(;i<tag.length;i++)
  {
   c = *i;
   if (s==0)
   {
    if (c == 'b') s = 1;
    else if (c == 'f') s = 2;
    else return _TAG_CUSTOM;
   }
   else    if (s==1)
   {
    if (c == 'l') s = 3;
    else return _TAG_CUSTOM;
   }
   else if (s==2)
   {
    if (c == 'i') s = 4;
    else return _TAG_CUSTOM;
   }
   else if (s==3)
   {
    if (c == 'o') s = 5;
    else return _TAG_CUSTOM;
   }
   else if (s==4)
   {
    if (c == 'g') s = 6;
    else return _TAG_CUSTOM;
   }
   else if (s==5)
   {
    if (c == 'c') s = 7;
    else return _TAG_CUSTOM;
   }
   else if (s==6)
   {
    if (c == 'c') s = 8;
    else return _TAG_CUSTOM;
   }
   else if (s==7)
   {
    if (c == 'k') s = 9;
    else return _TAG_CUSTOM;
   }
   else if (s==8)
   {
    if (c == 'a') s = 10;
    else return _TAG_CUSTOM;
   }
   else if (s==9)
   {
    if (c == 'q') s = 11;
    else return _TAG_CUSTOM;
   }
   else if (s==10)
   {
    if (c == 'p') s = 12;
    else return _TAG_CUSTOM;
   }
   else if (s==11)
   {
    if (c == 'u') s = 13;
    else return _TAG_CUSTOM;
   }
   else if (s==12)
   {
    if (c == 't') s = 14;
    else return _TAG_CUSTOM;
   }
   else if (s==13)
   {
    if (c == 'o') s = 15;
    else return _TAG_CUSTOM;
   }
   else if (s==14)
   {
    if (c == 'i') s = 16;
    else return _TAG_CUSTOM;
   }
   else if (s==15)
   {
    if (c == 't') s = 17;
    else return _TAG_CUSTOM;
   }
   else if (s==16)
   {
    if (c == 'o') s = 18;
    else return _TAG_CUSTOM;
   }
   else if (s==17)
   {
    if (c == 'e') return TAG_BLOCKQUOTE;
    else return _TAG_CUSTOM;
   }
   else if (s==18)
   {
    if (c == 'n') return TAG_FIGCAPTION;
    else return _TAG_CUSTOM;
   }
  }
 }
 return _TAG_CUSTOM;
}
