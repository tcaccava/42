#include "Span.hpp"

std::string Span::Error::toPrint = "prova";

Span::Span(void) : _N(0), array(0)
{}

Span::Span(unsigned int N) : _N(N), array(0)
{}

Span::Span(const Span& ref) : array(0)
{
	_N = ref._N;
	for (size_t i = 0; i < ref.array.size(); i++)
		array.push_back(ref.array[i]);
}

Span&	Span::operator= (const Span& ref)
{
	_N =  ref._N;
	array.clear();
	for (size_t i = 0; i < ref.array.size(); i++)
		array.push_back(ref.array[i]);
	return (*this);
}

void	Span::addNumber(int toAdd)
{
	if (_N == 0)
		Error::throwException("Array is full");
	array.push_back(toAdd);
	_N--;
}

void	Span::addRange(int min, int max)
{
	for (int i = min; i <= max; i++)
	{
		try
		{
			addNumber(i);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			break ;
		}
	}
}

void	Span::addRange(std::vector<int>::iterator begin, std::vector<int>::iterator end)
{
	for (std::vector<int>::iterator it = begin; it < end; it++)
	{
		try
		{
			addNumber(*it);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			break ;
		}
	}
}

void	Span::printArray(void) const
{
	for (size_t i = 0; i < array.size(); i++)
		std::cout << array[i] << std::endl;
}

unsigned int Span::shortestSpan(void)
{
	if (array.size() < 2)
		Error::throwException("There are less than two elements");

	std::vector<int> sorted = array;
	std::sort(sorted.begin(), sorted.end());;
	unsigned int span = sorted[1] - sorted[0];
	for (size_t i = 0; i < sorted.size() - 1; i++)
		if (sorted[i + 1] - sorted[i] < (int)span)
			span = sorted[i + 1] - sorted[i];
	return (span);
}

unsigned int	Span::longestSpan(void)
{
	if (array.size() < 2)
		Error::throwException("There are less than two elements");
	return (*(std::max_element(array.begin(), array.end())) - *(std::min_element(array.begin(), array.end())));
}

void Span::Error::throwException(std::string toPrint)
{
	Span::Error::toPrint = toPrint;
	throw Error();
}

Span::Error::~Error() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW{}

const char*	Span::Error::what() const throw()
{
	return (toPrint.c_str());
}

Span::~Span(void)
{}