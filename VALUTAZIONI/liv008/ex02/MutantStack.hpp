#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <stack>
#include <list>

template<typename T>
class	MutantStack : public std::stack<T>
{
	public:
		MutantStack(void);
		MutantStack(const MutantStack& ref);
		MutantStack& operator= (const MutantStack& ref);
		~MutantStack(void);

		typedef typename std::stack<T>::container_type::iterator iterator;
		typedef typename std::stack<T>::container_type::reverse_iterator reverse;
		typedef typename std::stack<T>::container_type::const_iterator c;
		typedef typename std::stack<T>::container_type::const_reverse_iterator creverse;

		iterator begin();
		iterator end();
		reverse rbegin();
		reverse rend();
		c cbegin();
		c cend();
		creverse crbegin();
		creverse crend();
};

#include "MutantStack.tpp"