HTMLTagType getTagType(ConstStr tag)
{
int s = 0;
ConstStrIterator i = tag.iterate();
if (tag.length==1)
{
if (*i == 'a') return TAG_A;
else if (*i == 'b') return TAG_B;
else if (*i == 'i') return TAG_I;
else if (*i == 'p') return TAG_P;
else if (*i == 'q') return TAG_Q;
else if (*i == 's') return TAG_S;
else if (*i == 'u') return TAG_U;
return _TAG_CUSTOM;
}
if (tag.length==2)
{
for(;i<tag.length;i++)
{
if (s==0)
{
if (*i == 'b') s = 1;
else if (*i == 'd') s = 2;
else if (*i == 'e') s = 3;
else if (*i == 'h') s = 4;
else if (*i == 'l') s = 5;
else if (*i == 'o') s = 6;
else if (*i == 'r') s = 7;
else if (*i == 't') s = 8;
else if (*i == 'u') s = 9;
else return _TAG_CUSTOM;
}
else if (s==1)
{
if (*i == 'r') return TAG_BR;
else return _TAG_CUSTOM;
}
else if (s==2)
{
if (*i == 'd') return TAG_DD;
else if (*i == 'l') return TAG_DL;
else if (*i == 't') return TAG_DT;
else return _TAG_CUSTOM;
}
else if (s==3)
{
if (*i == 'm') return TAG_EM;
else return _TAG_CUSTOM;
}
else if (s==4)
{
if (*i == '1') return TAG_H1;
else if (*i == '2') return TAG_H2;
else if (*i == '3') return TAG_H3;
else if (*i == '4') return TAG_H4;
else if (*i == '5') return TAG_H5;
else if (*i == '6') return TAG_H6;
else if (*i == 'r') return TAG_HR;
else return _TAG_CUSTOM;
}
else if (s==5)
{
if (*i == 'i') return TAG_LI;
else return _TAG_CUSTOM;
}
else if (s==6)
{
if (*i == 'l') return TAG_OL;
else return _TAG_CUSTOM;
}
else if (s==7)
{
if (*i == 'p') return TAG_RP;
else if (*i == 't') return TAG_RT;
else return _TAG_CUSTOM;
}
else if (s==8)
{
if (*i == 'd') return TAG_TD;
else if (*i == 'h') return TAG_TH;
else if (*i == 'r') return TAG_TR;
else if (*i == 't') return TAG_TT;
else return _TAG_CUSTOM;
}
else if (s==9)
{
if (*i == 'l') return TAG_UL;
else return _TAG_CUSTOM;
}
}
}
if (tag.length==3)
{
for(;i<tag.length;i++)
{
if (s==0)
{
if (*i == 'b') s = 1;
else if (*i == 'c') s = 2;
else if (*i == 'd') s = 3;
else if (*i == 'i') s = 4;
else if (*i == 'k') s = 5;
else if (*i == 'm') s = 6;
else if (*i == 'n') s = 7;
else if (*i == 'p') s = 8;
else if (*i == 's') s = 9;
else if (*i == 'v') s = 10;
else if (*i == 'w') s = 11;
else return _TAG_CUSTOM;
}
else if (s==1)
{
if (*i == 'd') s = 12;
else if (*i == 'i') s = 13;
else return _TAG_CUSTOM;
}
else if (s==2)
{
if (*i == 'o') s = 14;
else return _TAG_CUSTOM;
}
else if (s==3)
{
if (*i == 'e') s = 15;
else if (*i == 'f') s = 16;
else if (*i == 'i') s = 17;
else return _TAG_CUSTOM;
}
else if (s==4)
{
if (*i == 'm') s = 18;
else if (*i == 'n') s = 19;
else return _TAG_CUSTOM;
}
else if (s==5)
{
if (*i == 'b') s = 20;
else return _TAG_CUSTOM;
}
else if (s==6)
{
if (*i == 'a') s = 21;
else return _TAG_CUSTOM;
}
else if (s==7)
{
if (*i == 'a') s = 22;
else return _TAG_CUSTOM;
}
else if (s==8)
{
if (*i == 'r') s = 23;
else return _TAG_CUSTOM;
}
else if (s==9)
{
if (*i == 'u') s = 24;
else return _TAG_CUSTOM;
}
else if (s==10)
{
if (*i == 'a') s = 25;
else return _TAG_CUSTOM;
}
else if (s==11)
{
if (*i == 'b') s = 26;
else return _TAG_CUSTOM;
}
else if (s==12)
{
if (*i == 'i') return TAG_BDI;
else if (*i == 'o') return TAG_BDO;
else return _TAG_CUSTOM;
}
else if (s==13)
{
if (*i == 'g') return TAG_BIG;
else return _TAG_CUSTOM;
}
else if (s==14)
{
if (*i == 'l') return TAG_COL;
else return _TAG_CUSTOM;
}
else if (s==15)
{
if (*i == 'l') return TAG_DEL;
else return _TAG_CUSTOM;
}
else if (s==16)
{
if (*i == 'n') return TAG_DFN;
else return _TAG_CUSTOM;
}
else if (s==17)
{
if (*i == 'r') return TAG_DIR;
else if (*i == 'v') return TAG_DIV;
else return _TAG_CUSTOM;
}
else if (s==18)
{
if (*i == 'g') return TAG_IMG;
else return _TAG_CUSTOM;
}
else if (s==19)
{
if (*i == 's') return TAG_INS;
else return _TAG_CUSTOM;
}
else if (s==20)
{
if (*i == 'd') return TAG_KBD;
else return _TAG_CUSTOM;
}
else if (s==21)
{
if (*i == 'p') return TAG_MAP;
else return _TAG_CUSTOM;
}
else if (s==22)
{
if (*i == 'v') return TAG_NAV;
else return _TAG_CUSTOM;
}
else if (s==23)
{
if (*i == 'e') return TAG_PRE;
else return _TAG_CUSTOM;
}
else if (s==24)
{
if (*i == 'b') return TAG_SUB;
else if (*i == 'p') return TAG_SUP;
else return _TAG_CUSTOM;
}
else if (s==25)
{
if (*i == 'r') return TAG_VAR;
else return _TAG_CUSTOM;
}
else if (s==26)
{
if (*i == 'r') return TAG_WBR;
else return _TAG_CUSTOM;
}
}
}
if (tag.length==4)
{
for(;i<tag.length;i++)
{
if (s==0)
{
if (*i == 'a') s = 1;
else if (*i == 'b') s = 2;
else if (*i == 'c') s = 3;
else if (*i == 'f') s = 4;
else if (*i == 'h') s = 5;
else if (*i == 'l') s = 6;
else if (*i == 'm') s = 7;
else if (*i == 'r') s = 8;
else if (*i == 's') s = 9;
else if (*i == 't') s = 10;
else return _TAG_CUSTOM;
}
else if (s==1)
{
if (*i == 'b') s = 11;
else if (*i == 'r') s = 12;
else return _TAG_CUSTOM;
}
else if (s==2)
{
if (*i == 'a') s = 13;
else if (*i == 'o') s = 14;
else return _TAG_CUSTOM;
}
else if (s==3)
{
if (*i == 'i') s = 15;
else if (*i == 'o') s = 16;
else return _TAG_CUSTOM;
}
else if (s==4)
{
if (*i == 'o') s = 17;
else return _TAG_CUSTOM;
}
else if (s==5)
{
if (*i == 'e') s = 18;
else if (*i == 't') s = 19;
else return _TAG_CUSTOM;
}
else if (s==6)
{
if (*i == 'i') s = 20;
else return _TAG_CUSTOM;
}
else if (s==7)
{
if (*i == 'a') s = 21;
else if (*i == 'e') s = 22;
else return _TAG_CUSTOM;
}
else if (s==8)
{
if (*i == 'u') s = 23;
else return _TAG_CUSTOM;
}
else if (s==9)
{
if (*i == 'a') s = 24;
else if (*i == 'p') s = 25;
else return _TAG_CUSTOM;
}
else if (s==10)
{
if (*i == 'i') s = 26;
else return _TAG_CUSTOM;
}
else if (s==11)
{
if (*i == 'b') s = 27;
else return _TAG_CUSTOM;
}
else if (s==12)
{
if (*i == 'e') s = 28;
else return _TAG_CUSTOM;
}
else if (s==13)
{
if (*i == 's') s = 29;
else return _TAG_CUSTOM;
}
else if (s==14)
{
if (*i == 'd') s = 30;
else return _TAG_CUSTOM;
}
else if (s==15)
{
if (*i == 't') s = 31;
else return _TAG_CUSTOM;
}
else if (s==16)
{
if (*i == 'd') s = 32;
else return _TAG_CUSTOM;
}
else if (s==17)
{
if (*i == 'n') s = 33;
else if (*i == 'r') s = 34;
else return _TAG_CUSTOM;
}
else if (s==18)
{
if (*i == 'a') s = 35;
else return _TAG_CUSTOM;
}
else if (s==19)
{
if (*i == 'm') s = 36;
else return _TAG_CUSTOM;
}
else if (s==20)
{
if (*i == 'n') s = 37;
else return _TAG_CUSTOM;
}
else if (s==21)
{
if (*i == 'i') s = 38;
else if (*i == 'r') s = 39;
else return _TAG_CUSTOM;
}
else if (s==22)
{
if (*i == 'n') s = 40;
else if (*i == 't') s = 41;
else return _TAG_CUSTOM;
}
else if (s==23)
{
if (*i == 'b') s = 42;
else return _TAG_CUSTOM;
}
else if (s==24)
{
if (*i == 'm') s = 43;
else return _TAG_CUSTOM;
}
else if (s==25)
{
if (*i == 'a') s = 44;
else return _TAG_CUSTOM;
}
else if (s==26)
{
if (*i == 'm') s = 45;
else return _TAG_CUSTOM;
}
else if (s==27)
{
if (*i == 'r') return TAG_ABBR;
else return _TAG_CUSTOM;
}
else if (s==28)
{
if (*i == 'a') return TAG_AREA;
else return _TAG_CUSTOM;
}
else if (s==29)
{
if (*i == 'e') return TAG_BASE;
else return _TAG_CUSTOM;
}
else if (s==30)
{
if (*i == 'y') return TAG_BODY;
else return _TAG_CUSTOM;
}
else if (s==31)
{
if (*i == 'e') return TAG_CITE;
else return _TAG_CUSTOM;
}
else if (s==32)
{
if (*i == 'e') return TAG_CODE;
else return _TAG_CUSTOM;
}
else if (s==33)
{
if (*i == 't') return TAG_FONT;
else return _TAG_CUSTOM;
}
else if (s==34)
{
if (*i == 'm') return TAG_FORM;
else return _TAG_CUSTOM;
}
else if (s==35)
{
if (*i == 'd') return TAG_HEAD;
else return _TAG_CUSTOM;
}
else if (s==36)
{
if (*i == 'l') return TAG_HTML;
else return _TAG_CUSTOM;
}
else if (s==37)
{
if (*i == 'k') return TAG_LINK;
else return _TAG_CUSTOM;
}
else if (s==38)
{
if (*i == 'n') return TAG_MAIN;
else return _TAG_CUSTOM;
}
else if (s==39)
{
if (*i == 'k') return TAG_MARK;
else return _TAG_CUSTOM;
}
else if (s==40)
{
if (*i == 'u') return TAG_MENU;
else return _TAG_CUSTOM;
}
else if (s==41)
{
if (*i == 'a') return TAG_META;
else return _TAG_CUSTOM;
}
else if (s==42)
{
if (*i == 'y') return TAG_RUBY;
else return _TAG_CUSTOM;
}
else if (s==43)
{
if (*i == 'p') return TAG_SAMP;
else return _TAG_CUSTOM;
}
else if (s==44)
{
if (*i == 'n') return TAG_SPAN;
else return _TAG_CUSTOM;
}
else if (s==45)
{
if (*i == 'e') return TAG_TIME;
else return _TAG_CUSTOM;
}
}
}
if (tag.length==5)
{
for(;i<tag.length;i++)
{
if (s==0)
{
if (*i == 'a') s = 1;
else if (*i == 'e') s = 2;
else if (*i == 'f') s = 3;
else if (*i == 'i') s = 4;
else if (*i == 'l') s = 5;
else if (*i == 'm') s = 6;
else if (*i == 'p') s = 7;
else if (*i == 's') s = 8;
else if (*i == 't') s = 9;
else if (*i == 'v') s = 10;
else return _TAG_CUSTOM;
}
else if (s==1)
{
if (*i == 's') s = 11;
else if (*i == 'u') s = 12;
else return _TAG_CUSTOM;
}
else if (s==2)
{
if (*i == 'm') s = 13;
else return _TAG_CUSTOM;
}
else if (s==3)
{
if (*i == 'r') s = 14;
else return _TAG_CUSTOM;
}
else if (s==4)
{
if (*i == 'n') s = 15;
else return _TAG_CUSTOM;
}
else if (s==5)
{
if (*i == 'a') s = 16;
else return _TAG_CUSTOM;
}
else if (s==6)
{
if (*i == 'e') s = 17;
else return _TAG_CUSTOM;
}
else if (s==7)
{
if (*i == 'a') s = 18;
else return _TAG_CUSTOM;
}
else if (s==8)
{
if (*i == 'm') s = 19;
else if (*i == 't') s = 20;
else return _TAG_CUSTOM;
}
else if (s==9)
{
if (*i == 'a') s = 21;
else if (*i == 'b') s = 22;
else if (*i == 'f') s = 23;
else if (*i == 'h') s = 24;
else if (*i == 'i') s = 25;
else if (*i == 'r') s = 26;
else return _TAG_CUSTOM;
}
else if (s==10)
{
if (*i == 'i') s = 27;
else return _TAG_CUSTOM;
}
else if (s==11)
{
if (*i == 'i') s = 28;
else return _TAG_CUSTOM;
}
else if (s==12)
{
if (*i == 'd') s = 29;
else return _TAG_CUSTOM;
}
else if (s==13)
{
if (*i == 'b') s = 30;
else return _TAG_CUSTOM;
}
else if (s==14)
{
if (*i == 'a') s = 31;
else return _TAG_CUSTOM;
}
else if (s==15)
{
if (*i == 'p') s = 32;
else return _TAG_CUSTOM;
}
else if (s==16)
{
if (*i == 'b') s = 33;
else return _TAG_CUSTOM;
}
else if (s==17)
{
if (*i == 't') s = 34;
else return _TAG_CUSTOM;
}
else if (s==18)
{
if (*i == 'r') s = 35;
else return _TAG_CUSTOM;
}
else if (s==19)
{
if (*i == 'a') s = 36;
else return _TAG_CUSTOM;
}
else if (s==20)
{
if (*i == 'y') s = 37;
else return _TAG_CUSTOM;
}
else if (s==21)
{
if (*i == 'b') s = 38;
else return _TAG_CUSTOM;
}
else if (s==22)
{
if (*i == 'o') s = 39;
else return _TAG_CUSTOM;
}
else if (s==23)
{
if (*i == 'o') s = 40;
else return _TAG_CUSTOM;
}
else if (s==24)
{
if (*i == 'e') s = 41;
else return _TAG_CUSTOM;
}
else if (s==25)
{
if (*i == 't') s = 42;
else return _TAG_CUSTOM;
}
else if (s==26)
{
if (*i == 'a') s = 43;
else return _TAG_CUSTOM;
}
else if (s==27)
{
if (*i == 'd') s = 44;
else return _TAG_CUSTOM;
}
else if (s==28)
{
if (*i == 'd') s = 45;
else return _TAG_CUSTOM;
}
else if (s==29)
{
if (*i == 'i') s = 46;
else return _TAG_CUSTOM;
}
else if (s==30)
{
if (*i == 'e') s = 47;
else return _TAG_CUSTOM;
}
else if (s==31)
{
if (*i == 'm') s = 48;
else return _TAG_CUSTOM;
}
else if (s==32)
{
if (*i == 'u') s = 49;
else return _TAG_CUSTOM;
}
else if (s==33)
{
if (*i == 'e') s = 50;
else return _TAG_CUSTOM;
}
else if (s==34)
{
if (*i == 'e') s = 51;
else return _TAG_CUSTOM;
}
else if (s==35)
{
if (*i == 'a') s = 52;
else return _TAG_CUSTOM;
}
else if (s==36)
{
if (*i == 'l') s = 53;
else return _TAG_CUSTOM;
}
else if (s==37)
{
if (*i == 'l') s = 54;
else return _TAG_CUSTOM;
}
else if (s==38)
{
if (*i == 'l') s = 55;
else return _TAG_CUSTOM;
}
else if (s==39)
{
if (*i == 'd') s = 56;
else return _TAG_CUSTOM;
}
else if (s==40)
{
if (*i == 'o') s = 57;
else return _TAG_CUSTOM;
}
else if (s==41)
{
if (*i == 'a') s = 58;
else return _TAG_CUSTOM;
}
else if (s==42)
{
if (*i == 'l') s = 59;
else return _TAG_CUSTOM;
}
else if (s==43)
{
if (*i == 'c') s = 60;
else return _TAG_CUSTOM;
}
else if (s==44)
{
if (*i == 'e') s = 61;
else return _TAG_CUSTOM;
}
else if (s==45)
{
if (*i == 'e') return TAG_ASIDE;
else return _TAG_CUSTOM;
}
else if (s==46)
{
if (*i == 'o') return TAG_AUDIO;
else return _TAG_CUSTOM;
}
else if (s==47)
{
if (*i == 'd') return TAG_EMBED;
else return _TAG_CUSTOM;
}
else if (s==48)
{
if (*i == 'e') return TAG_FRAME;
else return _TAG_CUSTOM;
}
else if (s==49)
{
if (*i == 't') return TAG_INPUT;
else return _TAG_CUSTOM;
}
else if (s==50)
{
if (*i == 'l') return TAG_LABEL;
else return _TAG_CUSTOM;
}
else if (s==51)
{
if (*i == 'r') return TAG_METER;
else return _TAG_CUSTOM;
}
else if (s==52)
{
if (*i == 'm') return TAG_PARAM;
else return _TAG_CUSTOM;
}
else if (s==53)
{
if (*i == 'l') return TAG_SMALL;
else return _TAG_CUSTOM;
}
else if (s==54)
{
if (*i == 'e') return TAG_STYLE;
else return _TAG_CUSTOM;
}
else if (s==55)
{
if (*i == 'e') return TAG_TABLE;
else return _TAG_CUSTOM;
}
else if (s==56)
{
if (*i == 'y') return TAG_TBODY;
else return _TAG_CUSTOM;
}
else if (s==57)
{
if (*i == 't') return TAG_TFOOT;
else return _TAG_CUSTOM;
}
else if (s==58)
{
if (*i == 'd') return TAG_THEAD;
else return _TAG_CUSTOM;
}
else if (s==59)
{
if (*i == 'e') return TAG_TITLE;
else return _TAG_CUSTOM;
}
else if (s==60)
{
if (*i == 'k') return TAG_TRACK;
else return _TAG_CUSTOM;
}
else if (s==61)
{
if (*i == 'o') return TAG_VIDEO;
else return _TAG_CUSTOM;
}
}
}
if (tag.length==6)
{
for(;i<tag.length;i++)
{
if (s==0)
{
if (*i == 'a') s = 1;
else if (*i == 'b') s = 2;
else if (*i == 'c') s = 3;
else if (*i == 'd') s = 4;
else if (*i == 'f') s = 5;
else if (*i == 'h') s = 6;
else if (*i == 'i') s = 7;
else if (*i == 'k') s = 8;
else if (*i == 'l') s = 9;
else if (*i == 'o') s = 10;
else if (*i == 's') s = 11;
else return _TAG_CUSTOM;
}
else if (s==1)
{
if (*i == 'p') s = 12;
else return _TAG_CUSTOM;
}
else if (s==2)
{
if (*i == 'u') s = 13;
else return _TAG_CUSTOM;
}
else if (s==3)
{
if (*i == 'a') s = 14;
else if (*i == 'e') s = 15;
else return _TAG_CUSTOM;
}
else if (s==4)
{
if (*i == 'i') s = 16;
else return _TAG_CUSTOM;
}
else if (s==5)
{
if (*i == 'i') s = 17;
else if (*i == 'o') s = 18;
else return _TAG_CUSTOM;
}
else if (s==6)
{
if (*i == 'e') s = 19;
else return _TAG_CUSTOM;
}
else if (s==7)
{
if (*i == 'f') s = 20;
else return _TAG_CUSTOM;
}
else if (s==8)
{
if (*i == 'e') s = 21;
else return _TAG_CUSTOM;
}
else if (s==9)
{
if (*i == 'e') s = 22;
else return _TAG_CUSTOM;
}
else if (s==10)
{
if (*i == 'b') s = 23;
else if (*i == 'p') s = 24;
else if (*i == 'u') s = 25;
else return _TAG_CUSTOM;
}
else if (s==11)
{
if (*i == 'c') s = 26;
else if (*i == 'e') s = 27;
else if (*i == 'o') s = 28;
else if (*i == 't') s = 29;
else return _TAG_CUSTOM;
}
else if (s==12)
{
if (*i == 'p') s = 30;
else return _TAG_CUSTOM;
}
else if (s==13)
{
if (*i == 't') s = 31;
else return _TAG_CUSTOM;
}
else if (s==14)
{
if (*i == 'n') s = 32;
else return _TAG_CUSTOM;
}
else if (s==15)
{
if (*i == 'n') s = 33;
else return _TAG_CUSTOM;
}
else if (s==16)
{
if (*i == 'a') s = 34;
else return _TAG_CUSTOM;
}
else if (s==17)
{
if (*i == 'g') s = 35;
else return _TAG_CUSTOM;
}
else if (s==18)
{
if (*i == 'o') s = 36;
else return _TAG_CUSTOM;
}
else if (s==19)
{
if (*i == 'a') s = 37;
else return _TAG_CUSTOM;
}
else if (s==20)
{
if (*i == 'r') s = 38;
else return _TAG_CUSTOM;
}
else if (s==21)
{
if (*i == 'y') s = 39;
else return _TAG_CUSTOM;
}
else if (s==22)
{
if (*i == 'g') s = 40;
else return _TAG_CUSTOM;
}
else if (s==23)
{
if (*i == 'j') s = 41;
else return _TAG_CUSTOM;
}
else if (s==24)
{
if (*i == 't') s = 42;
else return _TAG_CUSTOM;
}
else if (s==25)
{
if (*i == 't') s = 43;
else return _TAG_CUSTOM;
}
else if (s==26)
{
if (*i == 'r') s = 44;
else return _TAG_CUSTOM;
}
else if (s==27)
{
if (*i == 'l') s = 45;
else return _TAG_CUSTOM;
}
else if (s==28)
{
if (*i == 'u') s = 46;
else return _TAG_CUSTOM;
}
else if (s==29)
{
if (*i == 'r') s = 47;
else return _TAG_CUSTOM;
}
else if (s==30)
{
if (*i == 'l') s = 48;
else return _TAG_CUSTOM;
}
else if (s==31)
{
if (*i == 't') s = 49;
else return _TAG_CUSTOM;
}
else if (s==32)
{
if (*i == 'v') s = 50;
else return _TAG_CUSTOM;
}
else if (s==33)
{
if (*i == 't') s = 51;
else return _TAG_CUSTOM;
}
else if (s==34)
{
if (*i == 'l') s = 52;
else return _TAG_CUSTOM;
}
else if (s==35)
{
if (*i == 'u') s = 53;
else return _TAG_CUSTOM;
}
else if (s==36)
{
if (*i == 't') s = 54;
else return _TAG_CUSTOM;
}
else if (s==37)
{
if (*i == 'd') s = 55;
else return _TAG_CUSTOM;
}
else if (s==38)
{
if (*i == 'a') s = 56;
else return _TAG_CUSTOM;
}
else if (s==39)
{
if (*i == 'g') s = 57;
else return _TAG_CUSTOM;
}
else if (s==40)
{
if (*i == 'e') s = 58;
else return _TAG_CUSTOM;
}
else if (s==41)
{
if (*i == 'e') s = 59;
else return _TAG_CUSTOM;
}
else if (s==42)
{
if (*i == 'i') s = 60;
else return _TAG_CUSTOM;
}
else if (s==43)
{
if (*i == 'p') s = 61;
else return _TAG_CUSTOM;
}
else if (s==44)
{
if (*i == 'i') s = 62;
else return _TAG_CUSTOM;
}
else if (s==45)
{
if (*i == 'e') s = 63;
else return _TAG_CUSTOM;
}
else if (s==46)
{
if (*i == 'r') s = 64;
else return _TAG_CUSTOM;
}
else if (s==47)
{
if (*i == 'i') s = 65;
else if (*i == 'o') s = 66;
else return _TAG_CUSTOM;
}
else if (s==48)
{
if (*i == 'e') s = 67;
else return _TAG_CUSTOM;
}
else if (s==49)
{
if (*i == 'o') s = 68;
else return _TAG_CUSTOM;
}
else if (s==50)
{
if (*i == 'a') s = 69;
else return _TAG_CUSTOM;
}
else if (s==51)
{
if (*i == 'e') s = 70;
else return _TAG_CUSTOM;
}
else if (s==52)
{
if (*i == 'o') s = 71;
else return _TAG_CUSTOM;
}
else if (s==53)
{
if (*i == 'r') s = 72;
else return _TAG_CUSTOM;
}
else if (s==54)
{
if (*i == 'e') s = 73;
else return _TAG_CUSTOM;
}
else if (s==55)
{
if (*i == 'e') s = 74;
else return _TAG_CUSTOM;
}
else if (s==56)
{
if (*i == 'm') s = 75;
else return _TAG_CUSTOM;
}
else if (s==57)
{
if (*i == 'e') s = 76;
else return _TAG_CUSTOM;
}
else if (s==58)
{
if (*i == 'n') s = 77;
else return _TAG_CUSTOM;
}
else if (s==59)
{
if (*i == 'c') s = 78;
else return _TAG_CUSTOM;
}
else if (s==60)
{
if (*i == 'o') s = 79;
else return _TAG_CUSTOM;
}
else if (s==61)
{
if (*i == 'u') s = 80;
else return _TAG_CUSTOM;
}
else if (s==62)
{
if (*i == 'p') s = 81;
else return _TAG_CUSTOM;
}
else if (s==63)
{
if (*i == 'c') s = 82;
else return _TAG_CUSTOM;
}
else if (s==64)
{
if (*i == 'c') s = 83;
else return _TAG_CUSTOM;
}
else if (s==65)
{
if (*i == 'k') s = 84;
else return _TAG_CUSTOM;
}
else if (s==66)
{
if (*i == 'n') s = 85;
else return _TAG_CUSTOM;
}
else if (s==67)
{
if (*i == 't') return TAG_APPLET;
else return _TAG_CUSTOM;
}
else if (s==68)
{
if (*i == 'n') return TAG_BUTTON;
else return _TAG_CUSTOM;
}
else if (s==69)
{
if (*i == 's') return TAG_CANVAS;
else return _TAG_CUSTOM;
}
else if (s==70)
{
if (*i == 'r') return TAG_CENTER;
else return _TAG_CUSTOM;
}
else if (s==71)
{
if (*i == 'g') return TAG_DIALOG;
else return _TAG_CUSTOM;
}
else if (s==72)
{
if (*i == 'e') return TAG_FIGURE;
else return _TAG_CUSTOM;
}
else if (s==73)
{
if (*i == 'r') return TAG_FOOTER;
else return _TAG_CUSTOM;
}
else if (s==74)
{
if (*i == 'r') return TAG_HEADER;
else return _TAG_CUSTOM;
}
else if (s==75)
{
if (*i == 'e') return TAG_IFRAME;
else return _TAG_CUSTOM;
}
else if (s==76)
{
if (*i == 'n') return TAG_KEYGEN;
else return _TAG_CUSTOM;
}
else if (s==77)
{
if (*i == 'd') return TAG_LEGEND;
else return _TAG_CUSTOM;
}
else if (s==78)
{
if (*i == 't') return TAG_OBJECT;
else return _TAG_CUSTOM;
}
else if (s==79)
{
if (*i == 'n') return TAG_OPTION;
else return _TAG_CUSTOM;
}
else if (s==80)
{
if (*i == 't') return TAG_OUTPUT;
else return _TAG_CUSTOM;
}
else if (s==81)
{
if (*i == 't') return TAG_SCRIPT;
else return _TAG_CUSTOM;
}
else if (s==82)
{
if (*i == 't') return TAG_SELECT;
else return _TAG_CUSTOM;
}
else if (s==83)
{
if (*i == 'e') return TAG_SOURCE;
else return _TAG_CUSTOM;
}
else if (s==84)
{
if (*i == 'e') return TAG_STRIKE;
else return _TAG_CUSTOM;
}
else if (s==85)
{
if (*i == 'g') return TAG_STRONG;
else return _TAG_CUSTOM;
}
}
}
if (tag.length==7)
{
for(;i<tag.length;i++)
{
if (s==0)
{
if (*i == 'a') s = 1;
else if (*i == 'c') s = 2;
else if (*i == 'd') s = 3;
else if (*i == 's') s = 4;
else return _TAG_CUSTOM;
}
else if (s==1)
{
if (*i == 'c') s = 5;
else if (*i == 'd') s = 6;
else if (*i == 'r') s = 7;
else return _TAG_CUSTOM;
}
else if (s==2)
{
if (*i == 'a') s = 8;
else return _TAG_CUSTOM;
}
else if (s==3)
{
if (*i == 'e') s = 9;
else return _TAG_CUSTOM;
}
else if (s==4)
{
if (*i == 'e') s = 10;
else if (*i == 'u') s = 11;
else return _TAG_CUSTOM;
}
else if (s==5)
{
if (*i == 'r') s = 12;
else return _TAG_CUSTOM;
}
else if (s==6)
{
if (*i == 'd') s = 13;
else return _TAG_CUSTOM;
}
else if (s==7)
{
if (*i == 't') s = 14;
else return _TAG_CUSTOM;
}
else if (s==8)
{
if (*i == 'p') s = 15;
else return _TAG_CUSTOM;
}
else if (s==9)
{
if (*i == 't') s = 16;
else return _TAG_CUSTOM;
}
else if (s==10)
{
if (*i == 'c') s = 17;
else return _TAG_CUSTOM;
}
else if (s==11)
{
if (*i == 'm') s = 18;
else return _TAG_CUSTOM;
}
else if (s==12)
{
if (*i == 'o') s = 19;
else return _TAG_CUSTOM;
}
else if (s==13)
{
if (*i == 'r') s = 20;
else return _TAG_CUSTOM;
}
else if (s==14)
{
if (*i == 'i') s = 21;
else return _TAG_CUSTOM;
}
else if (s==15)
{
if (*i == 't') s = 22;
else return _TAG_CUSTOM;
}
else if (s==16)
{
if (*i == 'a') s = 23;
else return _TAG_CUSTOM;
}
else if (s==17)
{
if (*i == 't') s = 24;
else return _TAG_CUSTOM;
}
else if (s==18)
{
if (*i == 'm') s = 25;
else return _TAG_CUSTOM;
}
else if (s==19)
{
if (*i == 'n') s = 26;
else return _TAG_CUSTOM;
}
else if (s==20)
{
if (*i == 'e') s = 27;
else return _TAG_CUSTOM;
}
else if (s==21)
{
if (*i == 'c') s = 28;
else return _TAG_CUSTOM;
}
else if (s==22)
{
if (*i == 'i') s = 29;
else return _TAG_CUSTOM;
}
else if (s==23)
{
if (*i == 'i') s = 30;
else return _TAG_CUSTOM;
}
else if (s==24)
{
if (*i == 'i') s = 31;
else return _TAG_CUSTOM;
}
else if (s==25)
{
if (*i == 'a') s = 32;
else return _TAG_CUSTOM;
}
else if (s==26)
{
if (*i == 'y') s = 33;
else return _TAG_CUSTOM;
}
else if (s==27)
{
if (*i == 's') s = 34;
else return _TAG_CUSTOM;
}
else if (s==28)
{
if (*i == 'l') s = 35;
else return _TAG_CUSTOM;
}
else if (s==29)
{
if (*i == 'o') s = 36;
else return _TAG_CUSTOM;
}
else if (s==30)
{
if (*i == 'l') s = 37;
else return _TAG_CUSTOM;
}
else if (s==31)
{
if (*i == 'o') s = 38;
else return _TAG_CUSTOM;
}
else if (s==32)
{
if (*i == 'r') s = 39;
else return _TAG_CUSTOM;
}
else if (s==33)
{
if (*i == 'm') return TAG_ACRONYM;
else return _TAG_CUSTOM;
}
else if (s==34)
{
if (*i == 's') return TAG_ADDRESS;
else return _TAG_CUSTOM;
}
else if (s==35)
{
if (*i == 'e') return TAG_ARTICLE;
else return _TAG_CUSTOM;
}
else if (s==36)
{
if (*i == 'n') return TAG_CAPTION;
else return _TAG_CUSTOM;
}
else if (s==37)
{
if (*i == 's') return TAG_DETAILS;
else return _TAG_CUSTOM;
}
else if (s==38)
{
if (*i == 'n') return TAG_SECTION;
else return _TAG_CUSTOM;
}
else if (s==39)
{
if (*i == 'y') return TAG_SUMMARY;
else return _TAG_CUSTOM;
}
}
}
if (tag.length==8)
{
for(;i<tag.length;i++)
{
if (s==0)
{
if (*i == '!') s = 1;
else if (*i == 'b') s = 2;
else if (*i == 'c') s = 3;
else if (*i == 'd') s = 4;
else if (*i == 'f') s = 5;
else if (*i == 'm') s = 6;
else if (*i == 'n') s = 7;
else if (*i == 'o') s = 8;
else if (*i == 'p') s = 9;
else if (*i == 't') s = 10;
else return _TAG_CUSTOM;
}
else if (s==1)
{
if (*i == 'd') s = 11;
else return _TAG_CUSTOM;
}
else if (s==2)
{
if (*i == 'a') s = 12;
else return _TAG_CUSTOM;
}
else if (s==3)
{
if (*i == 'o') s = 13;
else return _TAG_CUSTOM;
}
else if (s==4)
{
if (*i == 'a') s = 14;
else return _TAG_CUSTOM;
}
else if (s==5)
{
if (*i == 'i') s = 15;
else if (*i == 'r') s = 16;
else return _TAG_CUSTOM;
}
else if (s==6)
{
if (*i == 'e') s = 17;
else return _TAG_CUSTOM;
}
else if (s==7)
{
if (*i == 'o') s = 18;
else return _TAG_CUSTOM;
}
else if (s==8)
{
if (*i == 'p') s = 19;
else return _TAG_CUSTOM;
}
else if (s==9)
{
if (*i == 'r') s = 20;
else return _TAG_CUSTOM;
}
else if (s==10)
{
if (*i == 'e') s = 21;
else return _TAG_CUSTOM;
}
else if (s==11)
{
if (*i == 'o') s = 22;
else return _TAG_CUSTOM;
}
else if (s==12)
{
if (*i == 's') s = 23;
else return _TAG_CUSTOM;
}
else if (s==13)
{
if (*i == 'l') s = 24;
else return _TAG_CUSTOM;
}
else if (s==14)
{
if (*i == 't') s = 25;
else return _TAG_CUSTOM;
}
else if (s==15)
{
if (*i == 'e') s = 26;
else return _TAG_CUSTOM;
}
else if (s==16)
{
if (*i == 'a') s = 27;
else return _TAG_CUSTOM;
}
else if (s==17)
{
if (*i == 'n') s = 28;
else return _TAG_CUSTOM;
}
else if (s==18)
{
if (*i == 'f') s = 29;
else if (*i == 's') s = 30;
else return _TAG_CUSTOM;
}
else if (s==19)
{
if (*i == 't') s = 31;
else return _TAG_CUSTOM;
}
else if (s==20)
{
if (*i == 'o') s = 32;
else return _TAG_CUSTOM;
}
else if (s==21)
{
if (*i == 'x') s = 33;
else return _TAG_CUSTOM;
}
else if (s==22)
{
if (*i == 'c') s = 34;
else return _TAG_CUSTOM;
}
else if (s==23)
{
if (*i == 'e') s = 35;
else return _TAG_CUSTOM;
}
else if (s==24)
{
if (*i == 'g') s = 36;
else return _TAG_CUSTOM;
}
else if (s==25)
{
if (*i == 'a') s = 37;
else return _TAG_CUSTOM;
}
else if (s==26)
{
if (*i == 'l') s = 38;
else return _TAG_CUSTOM;
}
else if (s==27)
{
if (*i == 'm') s = 39;
else return _TAG_CUSTOM;
}
else if (s==28)
{
if (*i == 'u') s = 40;
else return _TAG_CUSTOM;
}
else if (s==29)
{
if (*i == 'r') s = 41;
else return _TAG_CUSTOM;
}
else if (s==30)
{
if (*i == 'c') s = 42;
else return _TAG_CUSTOM;
}
else if (s==31)
{
if (*i == 'g') s = 43;
else return _TAG_CUSTOM;
}
else if (s==32)
{
if (*i == 'g') s = 44;
else return _TAG_CUSTOM;
}
else if (s==33)
{
if (*i == 't') s = 45;
else return _TAG_CUSTOM;
}
else if (s==34)
{
if (*i == 't') s = 46;
else return _TAG_CUSTOM;
}
else if (s==35)
{
if (*i == 'f') s = 47;
else return _TAG_CUSTOM;
}
else if (s==36)
{
if (*i == 'r') s = 48;
else return _TAG_CUSTOM;
}
else if (s==37)
{
if (*i == 'l') s = 49;
else return _TAG_CUSTOM;
}
else if (s==38)
{
if (*i == 'd') s = 50;
else return _TAG_CUSTOM;
}
else if (s==39)
{
if (*i == 'e') s = 51;
else return _TAG_CUSTOM;
}
else if (s==40)
{
if (*i == 'i') s = 52;
else return _TAG_CUSTOM;
}
else if (s==41)
{
if (*i == 'a') s = 53;
else return _TAG_CUSTOM;
}
else if (s==42)
{
if (*i == 'r') s = 54;
else return _TAG_CUSTOM;
}
else if (s==43)
{
if (*i == 'r') s = 55;
else return _TAG_CUSTOM;
}
else if (s==44)
{
if (*i == 'r') s = 56;
else return _TAG_CUSTOM;
}
else if (s==45)
{
if (*i == 'a') s = 57;
else return _TAG_CUSTOM;
}
else if (s==46)
{
if (*i == 'y') s = 58;
else return _TAG_CUSTOM;
}
else if (s==47)
{
if (*i == 'o') s = 59;
else return _TAG_CUSTOM;
}
else if (s==48)
{
if (*i == 'o') s = 60;
else return _TAG_CUSTOM;
}
else if (s==49)
{
if (*i == 'i') s = 61;
else return _TAG_CUSTOM;
}
else if (s==50)
{
if (*i == 's') s = 62;
else return _TAG_CUSTOM;
}
else if (s==51)
{
if (*i == 's') s = 63;
else return _TAG_CUSTOM;
}
else if (s==52)
{
if (*i == 't') s = 64;
else return _TAG_CUSTOM;
}
else if (s==53)
{
if (*i == 'm') s = 65;
else return _TAG_CUSTOM;
}
else if (s==54)
{
if (*i == 'i') s = 66;
else return _TAG_CUSTOM;
}
else if (s==55)
{
if (*i == 'o') s = 67;
else return _TAG_CUSTOM;
}
else if (s==56)
{
if (*i == 'e') s = 68;
else return _TAG_CUSTOM;
}
else if (s==57)
{
if (*i == 'r') s = 69;
else return _TAG_CUSTOM;
}
else if (s==58)
{
if (*i == 'p') s = 70;
else return _TAG_CUSTOM;
}
else if (s==59)
{
if (*i == 'n') s = 71;
else return _TAG_CUSTOM;
}
else if (s==60)
{
if (*i == 'u') s = 72;
else return _TAG_CUSTOM;
}
else if (s==61)
{
if (*i == 's') s = 73;
else return _TAG_CUSTOM;
}
else if (s==62)
{
if (*i == 'e') s = 74;
else return _TAG_CUSTOM;
}
else if (s==63)
{
if (*i == 'e') s = 75;
else return _TAG_CUSTOM;
}
else if (s==64)
{
if (*i == 'e') s = 76;
else return _TAG_CUSTOM;
}
else if (s==65)
{
if (*i == 'e') s = 77;
else return _TAG_CUSTOM;
}
else if (s==66)
{
if (*i == 'p') s = 78;
else return _TAG_CUSTOM;
}
else if (s==67)
{
if (*i == 'u') s = 79;
else return _TAG_CUSTOM;
}
else if (s==68)
{
if (*i == 's') s = 80;
else return _TAG_CUSTOM;
}
else if (s==69)
{
if (*i == 'e') s = 81;
else return _TAG_CUSTOM;
}
else if (s==70)
{
if (*i == 'e') return TAG_DOCTYPE;
else return _TAG_CUSTOM;
}
else if (s==71)
{
if (*i == 't') return TAG_BASEFONT;
else return _TAG_CUSTOM;
}
else if (s==72)
{
if (*i == 'p') return TAG_COLGROUP;
else return _TAG_CUSTOM;
}
else if (s==73)
{
if (*i == 't') return TAG_DATALIST;
else return _TAG_CUSTOM;
}
else if (s==74)
{
if (*i == 't') return TAG_FIELDSET;
else return _TAG_CUSTOM;
}
else if (s==75)
{
if (*i == 't') return TAG_FRAMESET;
else return _TAG_CUSTOM;
}
else if (s==76)
{
if (*i == 'm') return TAG_MENUITEM;
else return _TAG_CUSTOM;
}
else if (s==77)
{
if (*i == 's') return TAG_NOFRAMES;
else return _TAG_CUSTOM;
}
else if (s==78)
{
if (*i == 't') return TAG_NOSCRIPT;
else return _TAG_CUSTOM;
}
else if (s==79)
{
if (*i == 'p') return TAG_OPTGROUP;
else return _TAG_CUSTOM;
}
else if (s==80)
{
if (*i == 's') return TAG_PROGRESS;
else return _TAG_CUSTOM;
}
else if (s==81)
{
if (*i == 'a') return TAG_TEXTAREA;
else return _TAG_CUSTOM;
}
}
}
if (tag.length==10)
{
for(;i<tag.length;i++)
{
if (s==0)
{
if (*i == 'b') s = 1;
else if (*i == 'f') s = 2;
else return _TAG_CUSTOM;
}
else if (s==1)
{
if (*i == 'l') s = 3;
else return _TAG_CUSTOM;
}
else if (s==2)
{
if (*i == 'i') s = 4;
else return _TAG_CUSTOM;
}
else if (s==3)
{
if (*i == 'o') s = 5;
else return _TAG_CUSTOM;
}
else if (s==4)
{
if (*i == 'g') s = 6;
else return _TAG_CUSTOM;
}
else if (s==5)
{
if (*i == 'c') s = 7;
else return _TAG_CUSTOM;
}
else if (s==6)
{
if (*i == 'c') s = 8;
else return _TAG_CUSTOM;
}
else if (s==7)
{
if (*i == 'k') s = 9;
else return _TAG_CUSTOM;
}
else if (s==8)
{
if (*i == 'a') s = 10;
else return _TAG_CUSTOM;
}
else if (s==9)
{
if (*i == 'q') s = 11;
else return _TAG_CUSTOM;
}
else if (s==10)
{
if (*i == 'p') s = 12;
else return _TAG_CUSTOM;
}
else if (s==11)
{
if (*i == 'u') s = 13;
else return _TAG_CUSTOM;
}
else if (s==12)
{
if (*i == 't') s = 14;
else return _TAG_CUSTOM;
}
else if (s==13)
{
if (*i == 'o') s = 15;
else return _TAG_CUSTOM;
}
else if (s==14)
{
if (*i == 'i') s = 16;
else return _TAG_CUSTOM;
}
else if (s==15)
{
if (*i == 't') s = 17;
else return _TAG_CUSTOM;
}
else if (s==16)
{
if (*i == 'o') s = 18;
else return _TAG_CUSTOM;
}
else if (s==17)
{
if (*i == 'e') return TAG_BLOCKQUOTE;
else return _TAG_CUSTOM;
}
else if (s==18)
{
if (*i == 'n') return TAG_FIGCAPTION;
else return _TAG_CUSTOM;
}
}
}
return _TAG_CUSTOM;
}

