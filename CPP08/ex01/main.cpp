#include "Span.hpp"

int main()
{
Span sp = Span(5);
sp.addNumber(6);
sp.addNumber(3);
sp.addNumber(17);
sp.addNumber(9);
sp.addNumber(11);
std::cout << sp.shortestSpan() << std::endl;
std::cout << sp.longestSpan() << std::endl;
std::vector<int> bigvector;
for(int i = 0; i < 10000; i++)
    bigvector.push_back(i * 2);
Span big(10000);
big.addNumbers(bigvector.begin(), bigvector.end());
std::cout << big.shortestSpan() << std::endl;
std::cout << big.longestSpan() << std::endl;
return 0;
}