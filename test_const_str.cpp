#include "const_str.h"
#include "settings.h"
#include <cstring>
#include <string>

int main()
{
	char* theString = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was born and I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness. No one rejects, dislikes, or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful. Nor again is there anyone who loves or pursues or desires to obtain pain of itself, because it is pain, but because occasionally circumstances occur in which toil and pain can procure him some great pleasure. To take a trivial example, which of us ever undertakes laborious physical exercise, except to obtain some advantage from it? But who has any right to find fault with a man who chooses to enjoy a pleasure that has no annoying consequences, or one who avoids a pain that produces no resultant pleasure?";
	int length = strlen(theString);
	printf("\nLength = %i\n", length);
	
	char** blocks = new char*[length/BLOCK_SIZE+1];
	char* storage = (char*)malloc(length+(length/BLOCK_SIZE)*16);
	// ^ 16 byte paddings in between blocks
	
	int b = 0;
	int c = 0;
	while (c<length)
	{
		blocks[b] = storage + b*(BLOCK_SIZE+16);
		if (c+BLOCK_SIZE<=length) memcpy(blocks[b], theString+c, BLOCK_SIZE);
		else memcpy(blocks[b], theString+c, length-c);
		b++;
		c += BLOCK_SIZE;
	}
	ConstStr whole(blocks, *blocks, length);
	////// done setting up... now the actual testing
	
	
	
	
	ConstStr part = whole.subString(3, 5);
	part.printLine();
	
	std::string test("em is");
	//printf("test=%s\n", test.c_str());
	if (part==test) printf("true\n");
	else printf("false\n");
	//if (asd)
	/*{
		printf("equalling\n");
		printf("false\n");
	}
	else printf("true\n");*/
}
