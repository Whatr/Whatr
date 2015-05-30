#include <string>

class ConstStr
{
public:
	char** startBlock;
	char* startChar;
	int length;
	ConstStr(char** startBlock, char* startChar, int length);
	//bool operator == (const ConstStr& str1, const std::string str2)
	bool operator == (ConstStr& str);
	char operator [] (int i);
	char* copy();
	void copyTo(char* startPos);
	ConstStr subString(int startPos, int lengthChars);
	void trim();
	void print();
	void printLine();
};
const bool operator == (const ConstStr& str1, const std::string& str2);
