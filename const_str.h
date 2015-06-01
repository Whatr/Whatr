#ifndef const_str_included
#define const_str_included yes

#include <string>
class ConstStrIterator;
class ConstStr
{
public:
	ConstStr* parent;
	
	char** startBlock;
	char* startChar;
	int length;
	
	ConstStr();
	ConstStr(char* str);
	ConstStr(char** startBlock, char* startChar, int length);
	ConstStr(char** startBlock, char* startChar, int length, ConstStr* parent);
	
	void operator = (const std::string& str);
	void operator = (const ConstStr& str);
	
	char* copy();
	void copyTo(char* startPos);
	ConstStr subString(int startPos, int lengthChars);
	ConstStr subString(int startPos);
	void trim();
	void print();
	void printLine();
	const char operator [] (const int i);
	
	bool isEmpty();
	
	int findReverse (const char* str);
	
	int findChar (int startPos, char target);
	int findChar (char target);
	
	ConstStrIterator iterate() const;
	ConstStrIterator iterate(const int startPos) const;
};
class ConstStrIterator
{
public:
	const ConstStr& cs;
	char** b1;
	char* c1;
	char* endC1;
	int pos;
	
	ConstStrIterator(const ConstStr& cs, const int startPos);
	
	ConstStr& constStr();
	
	char operator * ();
	int operator ++ (int);
	int operator -- (int);
	
};
const bool operator == (const ConstStr& str1, const std::string& str2);
const bool operator == (const ConstStr& str1, const ConstStr& str2);
const bool operator != (const ConstStr& str1, const std::string& str2);
const bool operator == (const ConstStrIterator& i1, const int& i2);
const bool operator != (const ConstStrIterator& i1, const int& i2);
const bool operator <  (const ConstStrIterator& i1, const int& i2);
const bool operator >  (const ConstStrIterator& i1, const int& i2);
const bool operator <= (const ConstStrIterator& i1, const int& i2);
const bool operator >= (const ConstStrIterator& i1, const int& i2);

/*
const bool operator != (const ConstStr& str1, const ConstStr& str2)
{
	return !(str1==str2);
}*/

#endif
