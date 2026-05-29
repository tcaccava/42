#include "MutantStack.hpp"

template<typename T>
MutantStack<T>::MutantStack(void) : std::stack<T>()
{}

template<typename T>
MutantStack<T>::MutantStack(const MutantStack& ref) : std::stack<T>(ref)
{}

template<typename T>
MutantStack<T>& MutantStack<T>::operator= (const MutantStack& ref)
{
	*this = ref;
	return (*this);
}

template <typename T>
MutantStack<T>::~MutantStack(void)
{}

template<typename T>
typename MutantStack<T>::iterator	MutantStack<T>::begin(void)
{
	return (std::stack<T>::c.begin());
}

template<typename T>
typename MutantStack<T>::iterator	MutantStack<T>::end(void)
{
	return (std::stack<T>::c.end());
}

template<typename T>
typename MutantStack<T>::reverse	MutantStack<T>::rbegin(void)
{
	return (std::stack<T>::c.rbegin());
}

template<typename T>
typename MutantStack<T>::reverse	MutantStack<T>::rend(void)
{
	return (std::stack<T>::c.rend());
}

template<typename T>
typename MutantStack<T>::c	MutantStack<T>::cbegin(void)
{
	return (std::stack<T>::c.cbegin());
}

template<typename T>
typename MutantStack<T>::c	MutantStack<T>::cend(void)
{
	return (std::stack<T>::c.cend());
}

template<typename T>
typename MutantStack<T>::creverse	MutantStack<T>::crbegin(void)
{
	return (std::stack<T>::c.crbegin());
}

template<typename T>
typename MutantStack<T>::creverse	MutantStack<T>::crend(void)
{
	return (std::stack<T>::c.crend());
}