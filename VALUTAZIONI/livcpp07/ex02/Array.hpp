#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

template <typename T>
class Array
{
	public:
		Array(void);
		Array(unsigned int n);
		Array(const Array& ref);
		Array& operator= (const Array& ref);
		~Array(void);

		unsigned int	size(void) const;
		T& operator [](unsigned int num);
		class	outOfBondException: public std::exception
		{
			public:
				const char* what() const throw();
		};

	private:
		T* array;
		unsigned int len;
};

#include "Array.tpp"
