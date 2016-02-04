#include <iostream>
#include "MemoryAllocator.h"

int main()
{
	MemoryAllocator allocator;
	
	//int *pArr1 = (int*)allocator.MyMalloc(100 * sizeof(int));
	//int *pArr2 = (int*)allocator.MyMalloc(100 * sizeof(int));

	List<int> test;

	test.pushBack(1);//0
	test.pushBack(2);//1
	test.pushBack(3);//2
	test.pushBack(4);//3
	test.pushBack(5);//4
	test.pushBack(6);//5

	test.print();
	std::cout << std::endl;

	test.delElem(3);

	test.print();
	std::cout << std::endl;

	return 0;
}