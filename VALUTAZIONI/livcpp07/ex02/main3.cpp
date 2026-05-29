#include "Array.hpp"

int main()
{
	Array<std::string> prova(4);

	std::cout << prova.size() << std::endl;
	try
	{
		prova[0] = "ciao";
		prova[1] = "come";
		prova[2] = "stai";
		prova[3] = "?";
		for (unsigned int i = 0; i < prova.size() + 1; i++)
			std::cout << prova[i] << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
