#pragma once
#include "List.h"
#include <vector>
#include <bitset>

class MemmoryAllocator
{
private:
	char buffer[1025];
	List<List<size_t>> sizeGroups;

	void markAsAllocated(size_t pos);
	bool isAllocated(size_t pos);
	size_t getSize(size_t pos);
public:
	MemmoryAllocator();
	~MemmoryAllocator();

	char* MyMalloc(size_t Size);
	void MyFree(char* pBlock);
};

void MemmoryAllocator::markAsAllocated(size_t pos)
{
	char comparator = 1; // == 00000001	
	buffer[pos] |= comparator;
}

bool MemmoryAllocator::isAllocated(size_t pos)
{
	char comparator = 1; // == 00000001
	char temp = buffer[pos];
	temp &= comparator;

	if (temp == comparator)
		return true;
	else
		return false;
}

size_t MemmoryAllocator::getSize(size_t pos)
{
	char transformer = 1022; // == 11111110
	char temp = buffer[pos];
	temp &= transformer;

	return size_t(temp);
}

MemmoryAllocator::MemmoryAllocator()
{
	//Setting empty size:
	buffer[0] = 1024;

	//Creating vector of vectors to group indexes by size:
	List<size_t> lessThan16b, lessThan32b, lessThan64b, lessThan128b, biggerThan128b;
	
	sizeGroups.pushBack(lessThan16b);
	sizeGroups.pushBack(lessThan32b);
	sizeGroups.pushBack(lessThan64b);
	sizeGroups.pushBack(lessThan128b);
	sizeGroups.pushBack(biggerThan128b);

	sizeGroups[4].data.pushBack(0);
	
	/*for (size_t i = 0; i < 1024; i++)
	{
		std::cout << i << " - " << std::bitset<8>(buffer[i]) << std::endl;
	}*/
}

MemmoryAllocator::~MemmoryAllocator()
{

}

char* MemmoryAllocator::MyMalloc(size_t Size)
{
	for (size_t i = 0; i < 1024; i += 4)
	{
		if (!isAllocated(i) && getSize(i) >= Size)
		{

		}
		//std::cout << i << " - " << std::bitset<8>(buffer[i]) << std::endl;
	}

	return NULL;
}

void MemmoryAllocator::MyFree(char* pBlock)
{

}