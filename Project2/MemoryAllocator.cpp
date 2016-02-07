#include "MemoryAllocator.h"

void MemoryAllocator::markAsAllocated(char* header)
{
	int* temp = (int*)header;
	allocated.pushBack(header);

	if (*temp % 4 == 0)
		(*temp)++;
}

void MemoryAllocator::markAsAllocatedWithSize(char* header, size_t size)
{
	int* temp = (int*)header;
	allocated.pushBack(header);
	*temp = size + 1; //+1 for allocated; 33 == allocated block with size 32
}

void MemoryAllocator::markAsFree(char* header)
{
	int* temp = (int*)header;
	allocated.delElement(header);
	if (*temp % 4 != 0)
		(*temp)--;
}

void MemoryAllocator::markAsFreeWithSize(char* header, size_t size)
{
	if (size % 4 == 0)
	{
		allocated.delElement(header);
		int* temp = (int*)header;
		*temp = size;
	}
}

bool MemoryAllocator::isAllocated(const char* header) const
{
	int* temp = (int*)header;

	if (*temp % 4 != 0)
		return true;
	else
		return false;
}

bool MemoryAllocator::checkNextisAllocated(char* header)
{
	size_t size = getSize(header);

	int* nextHeader = (int*)(header + size + 4);

	if (*nextHeader < CAPACITY - 4)
	{

		if (*nextHeader % 4 != 0)
			return true;
		else
			return false;
	}
	
	return true;
}

size_t MemoryAllocator::getSize(const char* pBlock) const
{
	int* temp = (int*)pBlock;

	if (*temp % 4 == 0)
		return size_t(*temp);
	else
		return size_t(*temp - 1);
}

size_t MemoryAllocator::getSizeOfNext(char* pBlock) const
{
	size_t size = getSize(pBlock);
	return getSize(pBlock + size + 4);
}

void MemoryAllocator::insertInGroups(char* pBlock)
{
	//if allocated pBlock is accidentally given to insertInGroups
	if (isAllocated(pBlock))
		return;

	//else insert into correct size group:
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
	//Search for best block to match this size if there is any:
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

		//if there is memory left unused we split the block in two:
		if (Size < min)
		{
			//create new block:
			char* temp2 = (temp + Size + 4);
			size_t secondBlockSize = min - Size - 4;

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

void MemoryAllocator::removeFromGroups(char* header)
{
	size_t size = getSize(header);

	if (size == 4)
	{
		size_t groupSize = sizeGroups[0].getSize();
		for (size_t i = 0; i < groupSize; i++)
		{
			if (header == sizeGroups[0][i])
			{
				sizeGroups[0].delElem(i);
				break;
			}

		}
		return;
	}
	if (size <= 8)
	{
		size_t groupSize = sizeGroups[1].getSize();
		for (size_t i = 0; i < groupSize; i++)
		{
			if (header == sizeGroups[1][i])
			{
				sizeGroups[1].delElem(i);
				break;
			}
		}
		return;

	}
	if (size <= 16)
	{
		size_t groupSize = sizeGroups[2].getSize();
		for (size_t i = 0; i < groupSize; i++)
		{
			if (header == sizeGroups[2][i])
			{
				sizeGroups[2].delElem(i);
				break;
			}
		}
		return;
	}
	if (size <= 32)
	{
		size_t groupSize = sizeGroups[3].getSize();
		for (size_t i = 0; i < groupSize; i++)
		{
			if (header == sizeGroups[3][i])
			{
				sizeGroups[3].delElem(i);
				break;
			}
		}
		return;
	}

	size_t groupSize = sizeGroups[4].getSize();
	for (size_t i = 0; i < groupSize; i++)
	{
		if (header == sizeGroups[4][i])
		{
			sizeGroups[4].delElem(i);
			break;
		}
	}

}

void MemoryAllocator::checkFragmentation()
{
	size_t groupSize = sizeGroups.getSize();

	for (size_t i = 0; i < groupSize; i++)
	{
		for (size_t j = 0; j < sizeGroups[i].getSize(); j++)
		{
			//if next block of this free block is also free
			if (!checkNextisAllocated(sizeGroups[i][j]))
			{
				size_t nextBlockSize = getSizeOfNext(sizeGroups[i][j]);
				char* nextHead = sizeGroups[i][j] + nextBlockSize + 4;

				//Merge together:
				size_t sizeWithNext = getSize(sizeGroups[i][j]) + nextBlockSize + 4;
				markAsFreeWithSize(sizeGroups[i][j], sizeWithNext);

				//delete first block from groups:
				sizeGroups[i].delElem(j);
				//delete second block from groups:
				removeFromGroups(nextHead);

				//add new merged block in sizeGroups:
				insertInGroups(sizeGroups[i][j]);
			}
		}

	}

}

bool MemoryAllocator::allocatedByAllocator(char* header)
{
	size_t size = allocated.getSize();

	for (size_t i = 0; i < size; i++)
		if (allocated[i] == header)
			return true;

	return false;
}

MemoryAllocator::MemoryAllocator()
{
	// Setting empty size:
	int* temp = (int*)buffer;
	*temp = CAPACITY - 4; //leave 4 bytes for header

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
		sizeGroups.delElem(i);
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

	std::cerr << "MemoryAllocator at " << this << " is out of space!\n";
	return NULL;
}

void MemoryAllocator::MyFree(char* pBlock)
{
	//Check if allocated by allocator or if pointer is NULL:
	if (pBlock == NULL)
	{
		std::cerr << "Address given is a NULL pointer!\n";
		return;
	}
	//Go back 4 bytes to read the header of the block:
	char* header = (pBlock - 4);
	//Check if it's allocated by this allocator:
	if (!allocatedByAllocator(header))
	{
		std::cerr << "Not allocated by allocator at " << this << " !\n";
		return;
	}

	size_t size = getSize(header);

	//If the next block is free, we merge them together:
	if (checkNextisAllocated(header))
	{
		markAsFree(header);
		insertInGroups(header);
	}
	else
	{
		size_t sizeWithNext = size + getSizeOfNext(header) + 4;//+ 4 because we have one header less
		markAsFreeWithSize(header, sizeWithNext);
		insertInGroups(header);
	}

	checkFragmentation();
	
	//Setting pointer = NULL for safety reasons:
	pBlock = NULL;
}

void MemoryAllocator::printStatus() const
{
	std::cout << "Memory status: ";
	std::cout << "||";

	for (size_t i = 0; i < CAPACITY; i++)
	{
		if (buffer[i] != char(204))
		{
			const char* temp = &buffer[i];

			//Check allocation status and get size of block:
			bool allocated = isAllocated(temp);
			size_t allocatedSize = getSize(temp);			

			//Print info:			
			if (allocated)
				std::cout << "{HEAD-4B->ALLOCATED-> " << allocatedSize << "B} ";
			else
				std::cout << "{HEAD-4B->FREE-> " << allocatedSize << "B} ";

			//Move to next block:
			i = i + allocatedSize + 3;
		}	
	}

	std::cout << "||";
	std::cout << std::endl;
}