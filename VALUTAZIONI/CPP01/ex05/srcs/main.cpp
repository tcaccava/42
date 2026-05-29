#include "Harl.h"
#include "Harl.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << RED << "INVALID ARGUMENTS!" << RESET << std::endl;
		return (EXIT_FAILURE);
	}

	Harl harl = Harl();
	harl.complain(std::string(av[1]));
	return (0);
}
