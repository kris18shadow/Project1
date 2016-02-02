#include "List.h"

template<typename T>
elem<T>& List<T>::operator[](size_t index)
{
	if (index == 0)
		return first;

	elem<T>* temp = first;
	for (size_t i = 1; i < index; i++)
	{
		if (temp == NULL)
			return NULL;
		
		temp = temp->next;
	}

	return *temp;
}


template<typename T>
void List<T>::pushBack(T newElem)
{
	if (first != NULL && last != NULL)
	{
		elem<T>* temp = new elem<T>;
		temp.data = newElem;
		temp.prev = last;
		temp.next = NULL;

		last->next = temp;
		last = temp;
	}
	else
	if (first == NULL)
	{
		elem<T>* temp = new elem<T>;
		temp.data = newElem;
		first = temp;
		first.next = NULL;
		first.prev = NULL;
	}
	else
	if(first != NULL && last == NULL)
	{
		elem<T>* temp = new elem<T>;
		temp.data = newElem;
		temp.prev = first;
		temp.next = NULL;
		last = temp;
	}
}

template<typename T>
int List<T>::size()
{
	int count = 0;
	elem<T> *p = first;
	while (p)
	{
		count++;
		p = p->next;
	}
	return count;
}