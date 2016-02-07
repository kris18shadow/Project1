#pragma once
#include "List.h"
/////////////////////////////////////////////////////////////////////////////////
// Block organization:
//
// [{HEADER}->{DATA}]
//
//	Where HEADER is 4 bytes and contains size of DATA and allocation status.
//	If a block is free, HEADER contains size,
//	if a block is allocated, HEADER contains size + 1.
//	EXAMPLE:
//	HEADER->SIZE = 32  =>  free block with size 32 bytes
//	HEADER->SIZE = 33  =>  allocated block with size 32 bytes
//----------------------------------------------------------------------------
//	Memory organization:
//
// [{HEADER}->{DATA}] -> [{HEADER}->{DATA}] -> ... -> [{HEADER}->{DATA}]
//
//Where memory is with size CAPACITY
////////////////////////////////////////////////////////////////////////////////


const size_t CAPACITY = 64;

class MemoryAllocator
{
private:
	//char because 1 char = 1 byte:
	char buffer[CAPACITY];
	List<List<char*>> sizeGroups;
	List<char*> allocated;

	void markAsAllocated(char* pBlock);
	void markAsAllocatedWithSize(char* pBlock, size_t size);
	void markAsFree(char* pBlock);
	void markAsFreeWithSize(char* pBlock, size_t size);

	bool isAllocated(const char* pBlock) const;
	bool checkNextisAllocated(char* pBlock);

	size_t getSize(const char* pBlock) const;
	size_t getSizeOfNext(char* pBlock) const;

	void insertInGroups(char* pBlock);
	char* checkBestMatch(List<char*> &group, size_t Size);
	void removeFromGroups(char* header);
	void checkFragmentation();
	bool allocatedByAllocator(char* header);

public:
	MemoryAllocator();
	~MemoryAllocator();

	char* MyMalloc(size_t Size);
	void MyFree(char* pBlock);

	void printStatus() const;
};