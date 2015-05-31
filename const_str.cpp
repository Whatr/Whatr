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
	if (length!=0 || startChar!=NULL) throw TRIED_TO_ASSIGN_TO_CONSTANT;
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
	if (length!=0 || startChar!=NULL) throw TRIED_TO_ASSIGN_TO_CONSTANT;
	this->parent = str.parent;
	this->startBlock = str.startBlock;
	this->startChar = str.startChar;
	this->length = str.length;
}
const bool operator != (const ConstStr& str1, const std::string& str2)
{
	return !(str1==str2);
}

const bool operator == (const ConstStr& str1, const std::string& str2)
{
	if (str1.length!=str2.size()) return false;
	char** b1 = str1.startBlock;
	char* c1 = str1.startChar;
	char* endC1 = *b1 + BLOCK_SIZE; // position of last char in block + 1
	for (int i=0;i<str1.length;i++)
	{
		if (c1==endC1)
		{
			b1++;
			c1 = *b1;
			endC1 = c1 + BLOCK_SIZE;
		}
		if (*c1 != str2.at(i)) return false;
		c1++;
	}
	return true;
}

const bool operator == (const ConstStr& str1, const ConstStr& str2)
{
	if (str1.length!=str2.length) return false;
	char** b1 = str1.startBlock;
	char* c1 = str1.startChar;
	char** b2 = str2.startBlock;
	char* c2 = str2.startChar;
	char* endC1 = *b1 + BLOCK_SIZE; // position of last char in block + 1
	char* endC2 = *b2 + BLOCK_SIZE;
	
	for (int i=0;i<str1.length;i++)
	{
		if (c1==endC1)
		{
			b1++;
			c1 = *b1;
			endC1 = c1 + BLOCK_SIZE;
		}
		if (c2==endC2)
		{
			b2++;
			c2 = *b2;
			endC2 = c2 + BLOCK_SIZE;
		}
		if (*c1 != *c2) return false;
		c1++;
		c2++;
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
		int copyLength = c1 - *b1 + BLOCK_SIZE;
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
void ConstStr::trim() // Easy :)
{
	while (length>0)
	{
		if ((*this)[0]==' ' ||
			(*this)[0]=='\n' ||
			(*this)[0]=='\t' ||
			(*this)[0]=='\r')
		{
			startChar++;
			if (startChar==(BLOCK_SIZE+*startBlock))
			{
				startBlock++;
				startChar = *startBlock;
			}
			length--;
		}
		else break;
	}
	while (length>0)
	{
		if ((*this)[length-1]==' ' ||
			(*this)[length-1]=='\n' ||
			(*this)[length-1]=='\t' ||
			(*this)[length-1]=='\r')
		{
			length--;
		}
		else break;
	}
}
void ConstStr::print()
{
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
	
	for (int i=length-1 ; i>=strLength-1 ; i--)
	{
		for (int j=0;j<strLength;j++)
		{
			if ( (*this)[i-strLength+1+j] == str[j] )
			{
				if (j==strLength-1)
				{
					return i-strLength+1;
				}
			}
			else
			{
				break;
			}
		}
	}
	return -1;
}
int ConstStr::findChar(char target)
{
	return this->findChar(0, target);
}
int ConstStr::findChar(int startPos, char target)
{
	char** b1 = startBlock + (startPos / BLOCK_SIZE);
	char* c1 = *b1 + (startPos % BLOCK_SIZE);
	char* endC1 = *b1 + BLOCK_SIZE; // position of last char in block + 1
	for (int i=startPos;i<length;i++)
	{
		if (c1==endC1)
		{
			b1++;
			c1 = *b1;
			endC1 = c1 + BLOCK_SIZE;
		}
		
		if (*c1 == target) return i;
		
		c1++;
	}
	return -1;
}
ConstStrIterator ConstStr::iterate()
{
	return this->iterate(0);
}
ConstStrIterator ConstStr::iterate(int startPos)
{
	return ConstStrIterator(*this, startPos);
}


ConstStrIterator::ConstStrIterator(ConstStr& cs, int startPos):
	cs(cs)
{
	if (startPos<0 || startPos>=cs.length) throw OUT_OF_STRING_BOUNDS;
	pos = startPos;
	
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
void ConstStrIterator::operator ++ ()
{
	pos++;
	if (pos>=cs.length) throw OUT_OF_STRING_BOUNDS;
	c1++;
	if (c1==endC1)
	{
		b1++;
		c1 = *b1;
		endC1 = c1 + BLOCK_SIZE;
	}
}
void ConstStrIterator::operator -- ()
{
	pos--;
	if (pos<0) throw OUT_OF_STRING_BOUNDS;
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
}

