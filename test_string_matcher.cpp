/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#include "html_tag_types.h"
#include "const_str.h"
#include <cstring>
#include <string>
#include <chrono>

#include "_gen_tag_name_matcher.cpp"

using namespace std;

int main()
{
	printf("==== hello ====\n");
	printf("==== welcome to string match tester ====\n");
	ConstStr tag1("noscript");
	ConstStr tag2("blockquote");
	HTMLTagType result;
	auto start = std::chrono::high_resolution_clock::now();
	printf("==== matching 1 million times... ====\n");
	for (int i=0; i<1000*1000; i++)
	{
		result = getTagType(((i&1)==0)?tag1:tag2);
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto time = end-start;
	
	printf("It took %i us\n",
			std::chrono::duration_cast<std::chrono::microseconds>(time).count());
}
