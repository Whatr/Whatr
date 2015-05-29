#include <string>

class ConstStr
{
public:
	// This class should provide constant strings in the form of references
	// to parts of a big constant string:
	char* start;
	int length;
	
	ConstStr(char* startCp, int lengthI)
	{
		start = startCp;
		length = lengthI;
	}
	
	// String functions:
	bool operator == (ConstStr& str) // Equals
	{
		if (this->length!=str.length) return false;
		char* c1 = this->start;
		char* c2 = str.start;
		for (int i=0;i<this->length;i++)
		{
			if (*c1 != *c2) return false;
			c1++;
			c2++;
		}
		return true;
	}
	bool operator == (std::string& str) // Equals
	{
		if (this->length!=str.size()) return false;
		char* c1 = this->start;
		for (int i=0;i<this->length;i++)
		{
			if (*c1 != str.at(i)) return false;
			c1++;
		}
		return true;
	}
	char operator [] (int i) // Get char
	{
		if (i>=length || i<0) return 0; // not fancy enough
		return start[i];
	}
	char* copy() // Copy it
	{
		char* ret = (char*)malloc(length+1);
		for (int i=0;i<length;i++)
		{
			ret[i] = start[i];
		}
		ret[length] = 0; // Null terminator
	}
};
