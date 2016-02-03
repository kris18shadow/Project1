#pragma once
#include "List.h"
#include <vector>
#include <bitset>

class MemoryAllocator
{
private:
	char buffer[1025];
	List<List<char*>> sizeGroups;

	void markAsAllocated(size_t pos);
	void markAsFree(char* pBlock);
	void markAsFreeWithSize(char* pBlock, size_t size);

	bool isAllocated(size_t pos);
	bool checkNextisAllocated(char* pBlock);

	size_t getSize(size_t pos);
	size_t getSize(char* pBlock);
	size_t getSizeOfNext(char* pBlock);
public:
	MemoryAllocator();
	~MemoryAllocator();

	char* MyMalloc(size_t Size);
	void MyFree(char* pBlock);
};

void MemoryAllocator::markAsAllocated(size_t pos)
{
	char comparator = 1; // == 00000001	
	buffer[pos] |= comparator;
}

void MemoryAllocator::markAsFree(char* pBlock)
{
	char transformer = 1022; // == 11111110
	*pBlock &= transformer;
}

void MemoryAllocator::markAsFreeWithSize(char* pBlock, size_t size)
{
	if (size % 2 == 0)
	{		
		*pBlock = size;
		markAsFree(pBlock);
	}
}

bool MemoryAllocator::isAllocated(size_t pos)
{
	char comparator = 1; // == 00000001
	char temp = buffer[pos];
	temp &= comparator;

	if (temp == comparator)
		return true;
	else
		return false;
}

bool MemoryAllocator::checkNextisAllocated(char* pBlock)
{
	size_t size = getSize(pBlock);

	if (*(pBlock + size) != buffer[1024])
	{
		if (isAllocated(*(pBlock + size)))
			return true;
		else
			return false;
	}
	else
		return true;

}

size_t MemoryAllocator::getSize(char* pBlock)
{
	char transformer = 1022; // == 11111110
	char temp = *pBlock;
	temp &= transformer;

	return size_t(temp);
}

size_t MemoryAllocator::getSize(size_t pos)
{
	char transformer = 1022; // == 11111110
	char temp = buffer[pos];
	temp &= transformer;

	return size_t(temp);
}

size_t MemoryAllocator::getSizeOfNext(char* pBlock)
{
	size_t size = getSize(pBlock);

	if (*(pBlock + size) != buffer[1024])
		return getSize(*(pBlock + size));
	else
		return 0;
}

MemoryAllocator::MemoryAllocator()
{
	//Setting empty size:
	buffer[0] = 1024;

	//Creating list of lists to group empty indexes by size:
	List<char*> lessThan4b, lessThan8b, lessThan16b, lessThan32b, biggerThan32b;
	
	sizeGroups.pushBack(lessThan4b);
	sizeGroups.pushBack(lessThan8b);
	sizeGroups.pushBack(lessThan16b);
	sizeGroups.pushBack(lessThan32b);
	sizeGroups.pushBack(biggerThan32b);

	sizeGroups[4].data.pushBack(&buffer[0]);
	
	/*for (size_t i = 0; i < 1024; i++)
	{
		std::cout << i << " - " << std::bitset<8>(buffer[i]) << std::endl;
	}*/
}

MemoryAllocator::~MemoryAllocator()
{

}

char* MemoryAllocator::MyMalloc(size_t Size)
{
	if (Size < 4 && !(sizeGroups[0].data.getSize() == 0))
	{

	}
	if (Size < 8 && !(sizeGroups[1].data.getSize() == 0))
	{

	}
	if (Size < 16 && !(sizeGroups[2].data.getSize() == 0))
	{

	}
	if (Size < 32 && !(sizeGroups[3].data.getSize() == 0))
	{

	}
	else
	{
		if (!(sizeGroups[4].data.getSize() == 0))
		{
			size_t availableSize = size_t(sizeGroups[4].data[0].data);
			if (Size < availableSize - 4)
			{
				//vzemame kolkoto size ni trqbva
				//markvame go kato allocated
				//ostanalata chast q vrushtame nqkude v sizeGroups
				//vrushtame pointer kum tova koeto sme allocatnali

			}
		}
	}

	return NULL;
}

void MemoryAllocator::MyFree(char* pBlock)
{
	size_t size = getSize(pBlock);

	if (checkNextisAllocated(pBlock))
	{

		markAsFree(pBlock);

		if (size < 4)
		{
			sizeGroups[0].data.pushBack(pBlock);
		}
		else
		if (size < 8)
		{
			sizeGroups[1].data.pushBack(pBlock);
		}
		else
		if (size < 16)
		{
			sizeGroups[2].data.pushBack(pBlock);
		}
		else
		if (size < 32)
		{
			sizeGroups[3].data.pushBack(pBlock);
		}
		else
		{
			sizeGroups[4].data.pushBack(pBlock);
		}
	}
	else
	{
		size_t sizeWithNext = size + getSizeOfNext(pBlock);
		markAsFreeWithSize( pBlock, sizeWithNext );

		if (sizeWithNext < 4)
		{
			sizeGroups[0].data.pushBack(pBlock);
		}
		else
		if (sizeWithNext < 8)
		{
			sizeGroups[1].data.pushBack(pBlock);
		}
		else
		if (sizeWithNext < 16)
		{
			sizeGroups[2].data.pushBack(pBlock);
		}
		else
		if (sizeWithNext < 32)
		{
			sizeGroups[3].data.pushBack(pBlock);
		}
		else
		{
			sizeGroups[4].data.pushBack(pBlock);
		}
	}
}