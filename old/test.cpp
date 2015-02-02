#include <iostream>

class Bla
{
public:
	int* pointerToA;
	int* pointerToB;
	Bla(int* aPointer, int* bPointer):
		pointerToA(aPointer),
		pointerToB(bPointer)
	{
		//this->pointerToA = aPointer;
		//this->pointerToB = bPointer;
	}
};
int a = 5;
int b = 7;
int main()
{
	Bla testBla(&a, &b); // Segmentation fault
	std::cout << "a=" << *(testBla.pointerToA) << " and b=" << *(testBla.pointerToB) << "\n";
}
