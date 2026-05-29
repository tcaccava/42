#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>

template<typename T>
typename T::iterator easyfind(T& container, int toFind)
{
	typename T::iterator	element;

	element = find(container.begin(), container.end(), toFind);
	if (element == container.end())
		throw std::runtime_error("Element not found");
	return (element);
};
