#pragma once
#include "List.h"

const size_t CAPACITY = 1024;

class MemoryAllocator
{
private:
	char buffer[CAPACITY];
	List<List<char*>> sizeGroups;

	void markAsAllocated(char* pBlock);
	void markAsAllocatedWithSize(char* pBlock, size_t size);
	void markAsFree(char* pBlock);
	void markAsFreeWithSize(char* pBlock, size_t size);

	bool isAllocated(size_t pos);
	bool isAllocated(char* pBlock);
	bool checkNextisAllocated(char* pBlock);

	size_t getSize(char* pBlock);
	size_t getSizeOfNext(char* pBlock);

	void insertInGroups(char* pBlock);
	char* checkBestMatch(List<char*> &group, size_t Size);
public:
	MemoryAllocator();
	~MemoryAllocator();

	char* MyMalloc(size_t Size);
	void MyFree(char* pBlock);
};

void MemoryAllocator::markAsAllocated(char* pBlock)
{
	int* temp = (int*)pBlock;

	if (*temp % 4 == 0)
		(*temp)++;
}

void MemoryAllocator::markAsAllocatedWithSize(char* pBlock, size_t size)
{
	int* temp = (int*)pBlock;
	*temp = size + 1; //+1 for allocated; 33 == allocated block with size 32
}

void MemoryAllocator::markAsFree(char* pBlock)
{
	int* temp = (int*)pBlock;

	if (*temp % 4 != 0)
		(*temp)--;
}

void MemoryAllocator::markAsFreeWithSize(char* pBlock, size_t size)
{
	if (size % 4 == 0)
	{
		int* temp = (int*)pBlock;
		*temp = size;
	}
}

bool MemoryAllocator::isAllocated(size_t pos)
{
	if (pos < CAPACITY)
	{
		char* temp = &buffer[pos];
		int* status = (int*)temp;

		if (*status % 4 == 0)
			return false;		
	}

	return true;
}

bool MemoryAllocator::isAllocated(char* pBlock)
{
	int* temp = (int*)pBlock;

	if (*temp % 4 != 0)
		return true;
	else
		return false;
}

bool MemoryAllocator::checkNextisAllocated(char* pBlock)
{
	size_t size = getSize(pBlock);

	int* temp = (int*)(pBlock + size);

	if (*temp % 4 != 0)
		return true;
	else
		return false;

}

size_t MemoryAllocator::getSize(char* pBlock)
{
	int* temp = (int*)pBlock;

	if (*temp % 4 == 0)
		return size_t(*temp);
	else
		return size_t(*temp - 1);
}

size_t MemoryAllocator::getSizeOfNext(char* pBlock)
{
	size_t size = getSize(pBlock);
	return getSize(pBlock + size);
}

void MemoryAllocator::insertInGroups(char* pBlock)
{
	//if allocated pBlock is accidentally given to insertInGroups
	if (isAllocated(pBlock)) 
		return;

	size_t size = getSize(pBlock);

	if (size == 4)
		sizeGroups[0].pushBack(pBlock);
	else
	if (size <= 8)
		sizeGroups[1].pushBack(pBlock);
	else
	if (size <= 16)
		sizeGroups[2].pushBack(pBlock);
	else
	if (size <= 32)
		sizeGroups[3].pushBack(pBlock);
	else
		sizeGroups[4].pushBack(pBlock);
}

char* MemoryAllocator::checkBestMatch(List<char*> &group, size_t Size)
{
	//Search for best block if there is any:
	size_t min = 0;
	bool minFound = false;
	size_t indexOfBestMatch = 0;

	for (size_t i = 0; i < group.getSize(); i++)
	{
		if (!minFound)
		{
			if (getSize(group[i]) >= Size)
			{
				min = getSize(group[i]);
				indexOfBestMatch = i;
				minFound = true;
			}
		}
		else
		{
			if (getSize(group[i]) >= Size && getSize(group[i]) < min)
			{
				min = getSize(group[i]);
				indexOfBestMatch = i;
			}

		}
	}

	//If there is a block in the group which is big enough:
	if (min != 0)
	{

		char* temp = group[indexOfBestMatch];
		group.delElem(indexOfBestMatch);

		//if there is memory left unused:
		if (Size < min)
		{
			//create new block:
			char* temp2 = (temp + Size + 4);
			size_t secondBlockSize = min - Size;

			//return new block as free:
			markAsFreeWithSize(temp2, secondBlockSize);
			insertInGroups(temp2);
		}

		//mark temp as allocated
		markAsAllocatedWithSize(temp, Size);

		//return block to user for writing
		return (temp + 4);	
	}
	else
	{
		return NULL;
	}

}

MemoryAllocator::MemoryAllocator()
{
	// Setting empty size:
	int* temp = (int*)buffer;
	*temp = CAPACITY;

	//Creating list of lists to group indexes of empty blocks by size:
	List<char*>* noMoreThan4b = new List<char*>;
	List<char*>* noMoreThan8b = new List<char*>;
	List<char*>* noMoreThan16b = new List<char*>;
	List<char*>* noMoreThan32b = new List<char*>;
	List<char*>* biggerThan32b = new List<char*>;

	sizeGroups.pushBack(*noMoreThan4b);
	sizeGroups.pushBack(*noMoreThan8b);
	sizeGroups.pushBack(*noMoreThan16b);
	sizeGroups.pushBack(*noMoreThan32b);
	sizeGroups.pushBack(*biggerThan32b);

	//Adding first block to list with empty blocks:
	char* firstBlock = &buffer[0];
	insertInGroups(firstBlock);
}

MemoryAllocator::~MemoryAllocator()
{
	for (size_t i = 0; i < 5; i++)
	{
		sizeGroups.delElem(i);
	}
}

char* MemoryAllocator::MyMalloc(size_t Size)
{
	//we need a number which can be divided by 4:
	while (Size % 4 != 0)
	{
		Size++;
	}

	if (Size == 4 && sizeGroups[0].getSize() != 0)
	{
		//no need to check for bestmatch (all blocks from this group are with size 4bytes)
		char* temp = sizeGroups[0][0];
		sizeGroups[0].popFront();

		//mark temp as allocated
		markAsAllocatedWithSize(temp, Size);

		//return block to user for writing
		return (temp + 4);

	}

	if (Size <= 8 && sizeGroups[1].getSize() != 0)
	{
		char* temp = checkBestMatch(sizeGroups[1], Size);
		if (temp != NULL)
			return temp;
	}

	if (Size <= 16 && sizeGroups[2].getSize() != 0)
	{
		char* temp = checkBestMatch(sizeGroups[2], Size);
		if (temp != NULL)
			return temp;
	}

	if (Size <= 32 && sizeGroups[3].getSize() != 0)
	{
		char* temp = checkBestMatch(sizeGroups[3], Size);
		if (temp != NULL)
			return temp;
	}
	else
	{
		if (sizeGroups[4].getSize() != 0)
		{
			char* temp = checkBestMatch(sizeGroups[4], Size);
			if (temp != NULL)
				return temp;
		}
	}

	return NULL;
}

void MemoryAllocator::MyFree(char* pBlock)
{
	//TODO check if its allocated by you!

	size_t size = getSize(pBlock);

	//If the next block is free, we merge them together:
	if (checkNextisAllocated(pBlock))
	{
		markAsFree(pBlock);
		insertInGroups(pBlock);
	}
	else
	{
		size_t sizeWithNext = size + getSizeOfNext(pBlock) + 4; //+ 4 because we have one header less
		markAsFreeWithSize(pBlock, sizeWithNext);
		insertInGroups(pBlock);
	}
}