#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

class Span
{
	public:
		Span(void);
		Span(unsigned int N);
		Span(const Span& ref);
		Span& operator= (const Span& ref);
		~Span(void);

		void	addNumber(int toAdd);
		void	addRange(int min, int max);
		void	addRange(std::vector<int>::iterator begin, std::vector<int>::iterator end);
		unsigned int shortestSpan(void);
		unsigned int longestSpan(void);
		void	printArray(void) const;
		class Error: public std::exception
		{
			public:
				static std::string toPrint;
				static void throwException(std::string Print);
			private:
				const char* what() const throw();
				~Error() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
		};


	private:
		unsigned int _N;
		std::string Error;
		std::vector<int> array;
};
