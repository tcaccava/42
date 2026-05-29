#include "easyfind.hpp"

int main()
{
	std::vector<int> array;
	std::vector<int>::iterator it;

	for (int i = 0; i < 5; i++)
		array.push_back(i);

	try
	{
		it = easyfind(array, 3);
		std::cout << *it << std::endl;
		it = easyfind(array, 8);
		std::cout << *it << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::list<int> list;
	std::list<int>::iterator itl;

	for (int i = 5; i < 10; i++)
		list.push_front(i);

	try
	{
		itl = easyfind(list, 8);
		std::cout << *itl << std::endl;
		itl = easyfind(list, 3);
		std::cout << *itl << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}