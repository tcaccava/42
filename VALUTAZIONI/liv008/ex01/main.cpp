#include "Span.hpp"

int main()
{
	{
		Span prova(9999);

		prova.addRange(1, 100000);
		prova.printArray();
		std::cout << prova.shortestSpan() << std::endl;
	}
	{
		Span sp = Span(5);
		sp.addNumber(6);
		sp.addNumber(3);
		sp.addNumber(17);
		sp.addNumber(9);
		sp.addNumber(11);
		std::cout << sp.shortestSpan() << std::endl;
		std::cout << sp.longestSpan() << std::endl;
		return 0;
	}
}
