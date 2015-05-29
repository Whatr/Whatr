#include <string>

enum Exceptions
{
	INVALID_ARGUMENT='INAR',
	OUT_OF_STRING_BOUNDS='OOSB',
};

class ConstStr
{
public:
	// This class describes a constant string as a pointer
	// to a part of a larger constant string:
	char* start;
	int length;
	
	ConstStr(char* startPos, int lengthChars)
	{
		start = startPos;
		length = lengthChars;
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
		if (i>=length || i<0) throw OUT_OF_STRING_BOUNDS;
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
	void copyTo(char* startPos) // Copy it
	{
		for (int i=0;i<length;i++)
		{
			startPos[i] = start[i];
		}
	}
	void copyTo(ConstStr destination, int startPos) // Copy it
	{
		for (int i=0;i<length;i++)
		{
			destination.start[startPos+i] = start[i];
		}
	}
	ConstStr subString(int startPos, int lengthChars) // Easy :)
	{
		if (startPos<0 || lengthChars<0) throw INVALID_ARGUMENT;
		if (startPos+lengthChars>=length) throw OUT_OF_STRING_BOUNDS;
		return ConstStr(start+startPos, lengthChars);
	}
	void trim() // Easy :)
	{
		while (length>0)
		{
			if (start[0]==' ' ||
				start[0]=='\n' ||
				start[0]=='\t' ||
				start[0]=='\r')
			{
				start++;
				length--;
			}
			else break;
		}
		while (length>0)
		{
			if (start[length-1]==' ' ||
				start[length-1]=='\n' ||
				start[length-1]=='\t' ||
				start[length-1]=='\r')
			{
				length--;
			}
			else break;
		}
	}
};
