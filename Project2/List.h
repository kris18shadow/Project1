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
	void copyList(List const& other);
	
public:

	List() :start(NULL), end(NULL) {};
	~List();
	List(List<T> const& other);
	List& operator=(List const& other);
	T& operator[](size_t index);

	void pushBack(T newElem);
	void popFront();
	void delElem(size_t pos);
	size_t getSize() const;

	void print() const;
};

template<typename T>
void List<T>::copyList(List const& other)
{
	if (other.start == NULL)
	{
		this->start = NULL;
		return;
	}

	elem<T>* temp = other.start;

	while (temp)
	{
		this->pushBack(temp->data);
		temp = temp->next;
	}	
}

template<typename T>
List<T>::List(List<T> const& other)
{
	copyList(other);
}

template<typename T>
List<T>& List<T>::operator=(List const& other)
{
	if (this != &other)
	{
		copyList(other);
	}

	return *this;
}

template<typename T>
List<T>::~List()
{
	if (start != NULL)
	{
		while (start->next != NULL)
		{
			elem<T>* temp = start->next;
			delete start;
			start = temp;
		}

		delete start;
	}
}

template<typename T>
T& List<T>::operator[](size_t index)
{
	if (index == 0)
		return start->data;

	elem<T>* temp = start;
	if (temp == NULL)
	{
		elem<T>* newElem = new elem<T>;		
		pushBack(newElem->data);
		return newElem->data;
	}
	for (size_t i = 0; i < index; i++)
	{
		temp = temp->next;
	}

	return temp->data;
}


template<typename T>
void List<T>::pushBack(T newElem)
{
	if (start != NULL && end != NULL)
	{
		elem<T>* temp = new elem<T>;
		temp->data = newElem;
		temp->prev = end;
		temp->next = NULL;

		end->next = temp;
		end = temp;
	}
	else
		if (start == NULL)
		{
			elem<T>* temp = new elem<T>;
			temp->data = newElem;
			start = temp;
			start->next = NULL;
			start->prev = NULL;
		}
		else
			if (start != NULL && end == NULL)
			{
				elem<T>* temp = new elem<T>;
				temp->data = newElem;
				temp->prev = start;
				start->next = temp;
				temp->next = NULL;
				end = temp;
			}
}

template<typename T>
void List<T>::popFront()
{
	if (start != NULL)
	{
		elem<T>* temp;
		temp = start;
		start = start->next;
		delete start;
	}
}

template<typename T>
void List<T>::delElem(size_t pos)
{
	size_t currSize = getSize();
	if (currSize < pos)
		return;

	elem<T>* temp = start;

	//if pos is first element:
	if (pos == 0)
	{
		start = start->next;
		start->prev = NULL;
		delete temp;
		return;
	}

	//if pos is last element:
	if (pos == currSize - 1)
	{
		temp = end;
		end = end->prev;
		end->next = NULL;
		delete temp;
		return;
	}

	//if pos > 0 && pos is not last element:
	for (size_t i = 0; i < pos; i++)
		temp = temp->next;

	elem<T>* tempPrev = temp->prev;
	elem<T>* tempNext = temp->next;
	
	tempPrev->next = tempNext;
	tempNext->prev = tempPrev;	
	
	delete temp;
}

template<typename T>
size_t List<T>::getSize() const
{
	int count = 0;
	elem<T> *p = start;
	while (p)
	{
		count++;
		if (p->next != NULL)
			p = p->next;
		else
			break;
	}
	return count;
}

template<typename T>
void List<T>::print() const
{
	if (start != NULL)
	{
		elem<T>* temp = start;
		while (temp != end)
		{
			std::cout << " {" << temp->data << "} ";
			temp = temp->next;
		}
		std::cout << " {" << end->data << "} ";
	}
	else
		return;

	
}