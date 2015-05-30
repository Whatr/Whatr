#include "const_str.h"
#include "settings.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <cstring>

enum Exceptions
{
	INVALID_ARGUMENT='INAR',
	OUT_OF_STRING_BOUNDS='OOSB',
};



using namespace std;



//class ConstStr
//{
//public:
	// This class describes a constant string as a pointer
	// to a part of a larger constant string which is stored
	// in a number of blocks pointed to by a list of pointers:
	
	ConstStr::ConstStr(char** startBlock, char* startChar, int length):
		startBlock(startBlock),
		startChar(startChar),
		length(length){};
	
	// String functions:
	
	const bool operator == (const ConstStr& str1, const std::string& str2)
	{
		if (str1.length!=str2.size())
		{
			return false;
		}
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
	
	bool ConstStr::operator == (ConstStr& str) // Equals
	{
		if (this->length!=str.length) return false;
		char** b1 = this->startBlock;
		char* c1 = this->startChar;
		char** b2 = str.startBlock;
		char* c2 = str.startChar;
		char* endC1 = *b1 + BLOCK_SIZE; // position of last char in block + 1
		char* endC2 = *b2 + BLOCK_SIZE;
		
		for (int i=0;i<this->length;i++)
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

	char ConstStr::operator [] (int i) // Get char
	{
		if (i>=length || i<0) throw OUT_OF_STRING_BOUNDS;
		return	*
				(
					(
						*
						(
							startBlock
							+
							(
								(startChar + i - *startBlock)
								/
								BLOCK_SIZE
							)
						)
					)
					+
					(
						(*startBlock - startChar + i)
						%
						BLOCK_SIZE
					)
				);
	}
	char* ConstStr::copy() // Copy it
	{
		char* ret = (char*)malloc(length+1);
		ret[length] = 0; // Null terminator
		this->copyTo(ret);
		return ret;
	}
	void ConstStr::copyTo(char* destination) // Copy it
	{
		char** b1 = this->startBlock;
		char* c1 = this->startChar;
		int progress = 0;
		if (*b1 != c1)
		{
			memcpy(destination, c1, c1 - *b1 + BLOCK_SIZE);
			progress += c1 - *b1;
			destination += c1 - *b1;
			b1++;
			c1 = *b1;
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
			memcpy(destination, c1, length - progress);
		}
	}
	ConstStr ConstStr::subString(int startPos, int lengthChars) // Easy :)
	{
		if (startPos<0 || lengthChars<0) throw INVALID_ARGUMENT;
		if (startPos+lengthChars>=length) throw OUT_OF_STRING_BOUNDS;
		
		char** newStartBlock = startBlock + (startChar-*startBlock+startPos) / BLOCK_SIZE;
		char* newStartChar = *newStartBlock + (startChar-*startBlock+startPos) % BLOCK_SIZE;
		return	ConstStr
				(
					newStartBlock,
					newStartChar,
					lengthChars
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
		this->print();
		printf("\n");
	}
//};
