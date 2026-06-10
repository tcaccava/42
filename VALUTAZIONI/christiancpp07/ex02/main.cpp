#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Array.hpp"

#define MAX_VAL 750

int main(void)
{
	Array<int> numbers(MAX_VAL);
	int* mirror = new int[MAX_VAL];
	srand(time(NULL));
	for (int i = 0; i < MAX_VAL; i++)
	{
		const int value = rand();
		numbers[i] = value;
		mirror[i] = value;
	}
	{
		Array<int> tmp = numbers;
		Array<int> test(tmp);
	}
	for (int i = 0; i < MAX_VAL; i++)
	{
		if (mirror[i] != numbers[i])
		{
			std::cerr << "didn't save the same value!!" << std::endl;
			delete[] mirror;
			return 1;
		}
	}
	try { numbers[-2] = 0; }
	catch (const std::exception& e) { std::cerr << e.what() << '\n'; }

	try { numbers[MAX_VAL] = 0; }
	catch (const std::exception& e) { std::cerr << e.what() << '\n'; }

	for (int i = 0; i < MAX_VAL; i++)
		numbers[i] = rand();

	Array<std::string> strs(3);
	strs[0] = "hello"; strs[1] = "world"; strs[2] = "cpp";
	std::cout << "size = " << strs.size() << std::endl;
	for (unsigned int i = 0; i < strs.size(); i++)
		std::cout << strs[i] << std::endl;

	Array<int> empty;
	std::cout << "empty size = " << empty.size() << std::endl;
	
	delete[] mirror;
	return 0;
}
