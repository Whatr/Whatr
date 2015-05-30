#include <string>

class ConstStr
{
public:
	char** startBlock;
	char* startChar;
	int length;
	ConstStr(char** startBlock, char* startChar, int length);
	bool operator == (ConstStr& str);
	bool operator == (std::string& str);
	char operator [] (int i);
	char* copy();
	void copyTo(char* startPos);
	ConstStr subString(int startPos, int lengthChars);
	void trim();
	void print();
	void printLine();
};
