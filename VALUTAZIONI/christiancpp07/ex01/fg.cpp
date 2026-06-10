#include <iostream>
#include "iter.hpp"

template <typename T>
void print(T const& x)
{
	std::cout << x << std::endl;
}

template <typename T>
void increment(T& x)
{
	x += 1;
}

int main(void)
{
	int arr[] = {1, 2, 3, 4, 5};
	std::cout << "-- print int array --" << std::endl;
	::iter(arr, 5, print<int>);

	std::cout << "-- increment int array --" << std::endl;
	::iter(arr, 5, increment<int>);
	::iter(arr, 5, print<int>);

	std::string words[] = {"hello", "world", "cpp"};
	std::cout << "-- print string array --" << std::endl;
	::iter(words, 3, print<std::string>);

	const int carr[] = {10, 20, 30};
	std::cout << "-- print const int array --" << std::endl;
	::iter(carr, 3, print<int>);

	return 0;
}
