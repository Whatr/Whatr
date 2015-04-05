class ConstStr
{
public:
	// This class should provide constant strings in the form of references
	// to parts of a big constant string:
	char* start;
	int length;
	
	// String functions:
	bool operator == (ConstStr& str2) // Equals
	{
		if (this->length!=str1.length) return false;
		char* c1 = this->start;
		char* c2 = str2.start;
		for (int i=0;i<this->length;i++)
		{
			if (*c1 != *c2) return false;
			c1++;
			c2++;
		}
		return true;
	}
	char operator [] (int i) // Get char
	{
		if (i>=length || i<0) return 0;
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
