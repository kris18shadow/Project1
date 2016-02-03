#pragma once
#include <iostream>

template<typename T>
struct elem
{
	T data;
	elem *prev, *next;
};

template<typename T>
class List
{
private:

	elem<T> *start, *end;

	//void deleteList();

public:

	List() :start(NULL), end(NULL) {};
	//~List();
	//List(List<T> const& other);
	//List& operator=(List const& other);
	elem<T>& operator[](size_t index);

	void pushBack(T newElem);
	void popFront();
	size_t getSize() const;
};