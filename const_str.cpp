#include "const_str.h"
#include "settings.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <cstring>

enum Exception1
{
	INVALID_ARGUMENT='INAR',
};
enum Execption2
{
	OUT_OF_STRING_BOUNDS='OOSB',
};
enum Exception3
{
	TRIED_TO_ASSIGN_TO_CONSTANT='TATC',
};

using namespace std;


ConstStr::ConstStr():
	startBlock(NULL),
	startChar(NULL),
	length(0),
	parent(NULL){};
ConstStr::ConstStr(char* str)
{
	char** blocks = new char*;
	*blocks = str;
	startBlock = blocks;
	startChar = str;
	length = strlen(str);
	parent = NULL;
}
ConstStr::ConstStr(char** startBlock, char* startChar, int length):
	startBlock(startBlock),
	startChar(startChar),
	length(length),
	parent(NULL){};
ConstStr::ConstStr(char** startBlock, char* startChar, int length, ConstStr* parent):
	startBlock(startBlock),
	startChar(startChar),
	length(length),
	parent(parent){};

void ConstStr::operator = (const std::string& str)
{
	//if (length!=0 || startChar!=NULL) throw TRIED_TO_ASSIGN_TO_CONSTANT;
	char* theStr = (char*)malloc(str.size()+1);
	strcpy(theStr, str.c_str());
	char** theBlock = new char*;
	*theBlock = theStr;
	startBlock = theBlock;
	startChar = theStr;
	length = str.size();
}
void ConstStr::operator = (const ConstStr& str)
{
	//if (length!=0 || startChar!=NULL) throw TRIED_TO_ASSIGN_TO_CONSTANT;
	this->parent = str.parent;
	this->startBlock = str.startBlock;
	this->startChar = str.startChar;
	this->length = str.length;
}
const bool operator != (const ConstStr& str1, const std::string& str2)
{
	return !(str1==str2);
}
const bool operator != (const ConstStr& str1, const ConstStr& str2)
{
	return !(str1==str2);
}
const bool operator == (const ConstStr& str1, const char& str2)
{
	return	( str1.length==1 ) &&
			( *(str1.startChar) == str2 );
}
const bool operator == (const ConstStr& str1, const std::string& str2)
{
	if (str1.length!=str2.size()) return false;
	
	ConstStrIterator i = str1.iterate();
	for (int j=0;j<str1.length;j++,i++)
	{
		if (*i != str2.at(j)) return false;
	}
	return true;
}

const bool ConstStr::operator -= (const std::string& str2) // Case insensitive cmp
{
	if (this->length!=str2.size()) return false;
	
	ConstStrIterator i = this->iterate();
	for (int j=0;j<this->length;j++,i++)
	{
		char c1 = *i;
		char c2 = str2.at(j);
		if (c1==c2) continue;
		if (c1>='A')
		{
			if (c1<='Z')
			{
				if (c1+('a'-'A') == c2) continue;
				return false;
			}
			if (c1>='a' && c1<='z')
			{
				if (c1-('a'-'A') == c2) continue;
				return false;
			}
		}
		return false;
	}
	return true;
}

const bool operator == (const ConstStr& str1, const ConstStr& str2)
{
	if (str1.length!=str2.length) return false;
	
	ConstStrIterator i1 = str1.iterate();
	ConstStrIterator i2 = str2.iterate();
	
	for (int i=0;i<str1.length;i++)
	{
		if (*i1 != *i2) return false;
		i1++;
		i2++;
	}
	return true;
}

const char ConstStr::operator [] (const int i) // Get char
{
	if (i>=length || i<0)
	{
		printf("ConstStr[] out of string bounds: i=%i length=%i\n", i, length);
		throw OUT_OF_STRING_BOUNDS;
	}
	return	* // lol
			(
				(
					* // lol
					(
						startBlock
						+
						(
							(startChar - *startBlock + i)
							/
							BLOCK_SIZE
						)
					)
				)
				+
				(
					(startChar - *startBlock + i)
					%
					BLOCK_SIZE
				)
			);
}
char* ConstStr::copy() // Copy it
{
	char* ret = (char*)malloc(length+1);
	if (ret==NULL)
	{
		printf("malloc failed!\n");
		throw 'SHIT';
	}
	ret[length] = 0; // Null terminator
	this->copyTo(ret);
	return ret;
}
void ConstStr::copyTo(char* destination) // Copy it
{
	char** b1 = startBlock;
	char* c1 = startChar;
	int progress = 0;
	if (*b1 != c1)
	{
		int copyLength = BLOCK_SIZE - (c1 - *b1);
		if (copyLength<=length)
		{
			memcpy(destination, c1, copyLength);
			progress += copyLength;//c1 - *b1;
			destination += copyLength;//c1 - *b1;
			b1++;
			c1 = *b1;
		}
		else
		{
			memcpy(destination, c1, length);
			return;
		}
	}
	while (length - progress >= BLOCK_SIZE)
	{
		memcpy(destination, c1, BLOCK_SIZE);
		progress += BLOCK_SIZE;
		destination += BLOCK_SIZE;
		b1++;
		c1 = *b1;
	}
	if (length > progress)
	{
		destination[0] = c1[0];
		memcpy(destination, c1, length - progress);
	}
}
ConstStr ConstStr::subString(int startPos) // Easy :)
{
	return this->subString(startPos, this->length-startPos);
}
ConstStr ConstStr::subString(int startPos, int lengthChars) // Easy :)
{
	if (startPos<0 || lengthChars<0) throw INVALID_ARGUMENT;
	if (startPos+lengthChars>length)
	{
		printf("ConstStr.subString out of string bounds: startPos=%i lengthChars=%i length=%i\n", startPos, lengthChars, length);
		throw OUT_OF_STRING_BOUNDS;
	}
	
	char** newStartBlock = startBlock + (startChar-*startBlock+startPos) / BLOCK_SIZE;
	char* newStartChar = *newStartBlock + (startChar-*startBlock+startPos) % BLOCK_SIZE;
	return	ConstStr
			(
				newStartBlock,
				newStartChar,
				lengthChars,
				this
			);
}
ConstStr ConstStr::trim(char c1, char c2, char c3, char c4) // Easy :)
{
	return this->trim(c1, c2, c3, c4, this->length);
}	
ConstStr ConstStr::trim(char c1, char c2, char c3, char c4, int maxChars) // Easy :)
{
	ConstStr ret = *this;
	int count = 0;
	while (ret.length>0 && count<maxChars)
	{
		if (ret[0]==c1 ||
			ret[0]==c2 ||
			ret[0]==c3 ||
			ret[0]==c4)
		{
			ret.startChar++;
			if (ret.startChar==(BLOCK_SIZE+*(ret.startBlock)))
			{
				ret.startBlock++;
				ret.startChar = *(ret.startBlock);
			}
			ret.length--;
			count++;
		}
		else break;
	}
	count = 0;
	while (ret.length>0 && count<maxChars)
	{
		if (ret[ret.length-1]==c1 ||
			ret[ret.length-1]==c2 ||
			ret[ret.length-1]==c3 ||
			ret[ret.length-1]==c4)
		{
			ret.length--;
			count++;
		}
		else break;
	}
	return ret;
}
void ConstStr::print()
{
	if (length==0) return;
	char** b1 = this->startBlock;
	char* c1 = this->startChar;
	int progress = 0;
	if (*b1 != c1)
	{
		int restOfBlockLength = BLOCK_SIZE - (c1 - *b1);
		if (length>=restOfBlockLength)
		{
			fwrite(c1, 1, restOfBlockLength, stdout);
			progress += restOfBlockLength;
			b1++;
			c1 = *b1;
		}
		else
		{
			fwrite(c1, 1, length, stdout);
			return;
		}
	}
	while (length - progress >= BLOCK_SIZE)
	{
		fwrite(c1, 1, BLOCK_SIZE, stdout);
		progress += BLOCK_SIZE;
		b1++;
		c1 = *b1;
	}
	if (length > progress)
	{
		fwrite(c1, 1, length-progress, stdout);
	}
}
void ConstStr::printLine()
{
	print();
	printf("\n");
}
bool ConstStr::isEmpty()
{
	return length==0;
}
int ConstStr::findReverse(const char* str)
{
	int strLength = strlen(str);
	
	for (ConstStrIterator i=iterate(length-strLength-1) ; i>=0 ; i--)
	{
		if (*i == str[0])
		{
			if (subString(i.pos, strLength)==str)
			{
				return i.pos;
			}
		}
	}
	return -1;
}


int ConstStr::find(char target)
{
	return this->find(0, target);
}
int ConstStr::find(int startPos, char target)
{
	for (ConstStrIterator i = this->iterate(startPos);i<length;i++)
	{
		if (*i == target) return i.pos;
	}
	return -1;
}


int ConstStr::find (ConstStr target)
{
	return find(0, target, (char)0);
}
int ConstStr::find (ConstStr target, char targetEscapeChar)
{
	return find(0, target, targetEscapeChar);
}
int ConstStr::find (int startPos, ConstStr target)
{
	return find(startPos, target, (char)0);
}
int ConstStr::find (int startPos, ConstStr target, char targetEscapeChar)
{
	if (target.length > this->length) return -1;
	if (target.length == this->length) return (target == *this) ? 0 : -1;
	ConstStrIterator i1 = this->iterate(startPos);	// bladibla
	ConstStrIterator i2 = target.iterate(); 		// dibs
	int count = 0;
	for (int pos=startPos;pos<this->length;pos++)
	{
		if (count==0)
		{
			if (*i1 == *i2)
			{
				count++;	// bladibla
			}				//    ^
			
		}
		else if (*i1 == *i2)
		{
			count++;		// bladibla
							//     ^^
			if (count==target.length) return pos-count+1;
		}
		else
		{
			i1.jump(pos-count+1);
			i2.backToStart();
			count = 0;
		}
		i1++;
		if (count>0) i2++;
	}
	return -1;
}

int ConstStr::toInt()
{
	char* endptr = startChar+length;
	return (int)strtol(startChar, &endptr, 10);
}
int ConstStr::toInt(bool* success)
{
	char* endptr = startChar+length;
	int ret = (int)strtol(startChar, &endptr, 10);
	if (endptr>startChar || ret!=0) *success = true;
	else *success = false;
	return ret;
}
int ConstStr::toInt(const int base)
{
	char* endptr = startChar+length;
	return (int)strtol(startChar, &endptr, base);
}
int ConstStr::toInt(const int base, bool* success)
{
	char* endptr = startChar+length;
	int ret = (int)strtol(startChar, &endptr, base);
	if (endptr>startChar || ret!=0) *success = true;
	else *success = false;
	return ret;
}
float ConstStr::toFloat()
{
	char* endptr = startChar+length;
	return strtof(startChar, &endptr);
}

std::ostream& operator << (std::ostream& o, ConstStr& str)
{
	str.print();
	return o;
}

ConstStrIterator ConstStr::iterate() const
{
	return this->iterate(0);
}
ConstStrIterator ConstStr::iterate(const int startPos) const
{
	return ConstStrIterator(*this, startPos);
}


ConstStrIterator::ConstStrIterator(const ConstStr& cs, const int startPos):
	cs(cs)
{
	if (startPos<0 || startPos>=cs.length) throw OUT_OF_STRING_BOUNDS;
	jump(startPos);
}
void ConstStrIterator::jump(int destination)
{
	pos = destination;
	
	b1 // current block
	=
	cs.startBlock
	+
	(
		(cs.startChar - *(cs.startBlock) + pos)
		/
		BLOCK_SIZE
	);
	
	c1 // current char
	=
	*b1
	+
	(
		(cs.startChar - *(cs.startBlock) + pos)
		%
		BLOCK_SIZE
	);
	
	endC1 = *b1 + BLOCK_SIZE; // position of last char in block + 1
}

ConstStr& constStr();

char ConstStrIterator::operator * () // Get current char
{
	return *c1;
}
int ConstStrIterator::operator ++ (int)
{
	pos++;
	if (pos>=cs.length) return pos;//throw OUT_OF_STRING_BOUNDS;
	c1++;
	if (c1==endC1)
	{
		b1++;
		c1 = *b1;
		endC1 = c1 + BLOCK_SIZE;
	}
	return pos;
}
int ConstStrIterator::operator -- (int)
{
	pos--;
	if (pos<0) return pos; //throw OUT_OF_STRING_BOUNDS;
	if (c1==*b1) // if we're at the start of a block
	{
		b1--; // go to previous block
		endC1 = *b1 + BLOCK_SIZE;
		c1 = endC1 - 1; // point to the last character of the block
	}
	else
	{
		c1--;
	}
	return pos;
}


void ConstStrIterator::backToStart()
{
	pos = 0;
	b1 = cs.startBlock;
	c1 = cs.startChar;
	endC1 = *b1 + BLOCK_SIZE;
}

const bool operator == (const ConstStrIterator& i1, const int& i2)
{ return i1.pos == i2; }
const bool operator != (const ConstStrIterator& i1, const int& i2)
{ return i1.pos != i2; }
const bool operator <  (const ConstStrIterator& i1, const int& i2)
{ return i1.pos <  i2; }
const bool operator >  (const ConstStrIterator& i1, const int& i2)
{ return i1.pos >  i2; }
const bool operator <= (const ConstStrIterator& i1, const int& i2)
{ return i1.pos <= i2; }
const bool operator >= (const ConstStrIterator& i1, const int& i2)
{ return i1.pos >= i2; }

