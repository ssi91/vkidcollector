//
// Created by ssi on 03.04.15.
//

#include "Stack.h"
#include "renderexception.h"

template< class T >
inline Stack< T >::Stack()
{
	count = 0;
}

template<>
inline Stack< int >::Stack(const int &_a)
{
	count = 1;
	a = new int;
	a[0] = _a;
}

template< class T >
inline Stack< T >::~Stack()
{
	if (!getCount())
		return;
	else
		delete[] a;
}

template< class T >
inline void Stack< T >::push(T const &_a)
{
	T *temp;
	if (count)
	{
		temp = new T[count];
		for (int i = 0; i < count; ++i)
		{
			temp[i] = a[i];
		}
		delete[] a;
	}
	a = new T[count + 1];
	if (count)
	{
		for (int i = 0; i < count; ++i)
		{
			a[i] = temp[i];
		}
		delete[] temp;
	}
	a[count] = _a;
	++count;
}

template< class T >
inline T Stack< T >::pop()
{
	if (count)
	{
		T top;
		top = a[count - 1];
		--count;
		if (count)
		{
			T *temp = new T[count];
			for (int i = 0; i < count; ++i)
			{
				temp[i] = a[i];
			}
			delete[] a;
			a = new T[count];
			for (int i = 0; i < count; ++i)
			{
				a[i] = temp[i];
			}
			delete[] temp;
		}
		else
		{
			delete[] a;
		}
		return top;
	}
}

template<>
inline void Stack< char >::incTop()
{
	if (count)
		++a[count - 1];
	else
	{
		throw LOG_TRACE("Stack is empty")
	}
}

template<>
inline void Stack< int >::incTop()
{
	if (count)
		++a[count - 1];
	else
	{
		throw LOG_TRACE("Stack is empty")
	}
}

template<>
inline void Stack< char >::decTop()
{
	if (count)
		--a[count - 1];
	else
	{
		throw LOG_TRACE("Stack is empty")
	}
}

template<>
inline void Stack< int >::decTop()
{
	if (count)
		--a[count - 1];
	else
	{
		throw LOG_TRACE("Stack is empty")
	}
}

template< class T >
inline size_t Stack< T >::getCount()
{
	return count;
}

template< class T >
inline T &Stack< T >::getTop()
{
	if (count)
	{
		return a[count - 1];
	}
}

template< class T >
Stack< T >::Stack(const Stack< T > &stack)
{
	count = stack.count;
	if (count)
	{
		a = new T[count];
		for (int i = 0; i < count; ++i)
		{
			a[i] = stack.a[i];
		}
	}
}

template< class T >
Stack< T > &Stack< T >::operator=(Stack< T > &stack)
{
	if (count)
		delete[] a;

	count = stack.count;
	if (count)
	{
		a = new T[count];
		for (int i = 0; i < count; ++i)
		{
			a[i] = stack.a[i];
		}
	}

	return *this;
}
