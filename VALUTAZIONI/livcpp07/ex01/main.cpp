#include "iter.hpp"

void	addOne(int&	num)
{
	num += 1;
}

void	printChar(const char& c)
{
	std::cout << c << std::endl;
}

int main()
{
	int	array[3] = {1, 2, 3};

	iter(array, 3, addOne);
	iter(array, 3, print<int>);

	char	str[] = "ciao";
	iter(str, 4, printChar);
	iter(str, 4, print<char>);

	double numbers[] = {22.0, 23, 24.05};
	iter
}
