#include <iostream>
#include "MemoryAllocator.h"

int main()
{
	std::cout << "Capacity of memory is set to: " << CAPACITY << std::endl << std::endl;

	MemoryAllocator allocator;
	
	std::cout << "INITIAL STATUS:\n";
	allocator.printStatus();
	std::cout << std::endl;
	
	///////////////////
	//ALLOCATING:
	//////////////////
	std::cout << "--Atempting llocation of 8*sizeof(int):\n";
	int *pArr1 = (int*)allocator.MyMalloc(8 * sizeof(int));
	allocator.printStatus();
	std::cout << std::endl;
	
	std::cout << "--Atempting Allocation of 2*sizeof(int):\n";
	int *pArr2 = (int*)allocator.MyMalloc(2 * sizeof(int));
	allocator.printStatus();
	std::cout << std::endl;

	std::cout << "--Atempting Allocation of 8*sizeof(int) (Must fail, because allocator is out of memory):\n";
	int *pArr3 = (int*)allocator.MyMalloc(8 * sizeof(int));
	allocator.printStatus();
	std::cout << std::endl;
	
	///////////////////
	//DELETING:
	//////////////////
	std::cout << "--DELETING:\n";

	std::cout << "\n--Freeing first allocated block:\n";
	allocator.MyFree((char*)pArr1);
	allocator.printStatus();
	std::cout << std::endl;

	std::cout << "\n--Freeing second allocated block:\n";
	allocator.MyFree((char*)pArr2);
	allocator.printStatus();
	std::cout << std::endl;

	allocator.MyFree((char*)pArr3); 
	allocator.printStatus();
	std::cout << std::endl;
	
		
	std::cout << "--Attempting to delete address which is not allocated by allocator:\n";
	int x = 25;
	int* parr5 = &x;
	allocator.MyFree((char*)parr5);


	return 0;
}