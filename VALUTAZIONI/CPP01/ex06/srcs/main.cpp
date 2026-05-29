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
	std::map<std::string, int> levels;
	int level;
	std::string input = av[1];
	
	levels["DEBUG"] = 1;
	levels["INFO"] = 2;
	levels["WARNING"] = 3;
	levels["ERROR"] = 4;


    levels.find(input) != levels.end() ? level = levels[input] : level = -1;
	
	switch (level)
	{
		default:
			harl.complain(input);
			break;
		case 1:
			harl.complain("DEBUG");
		// fall through
		case 2:
			harl.complain("INFO");
		// fall through
		case 3:
			harl.complain("WARNING");
		// fall through
		case 4:
			harl.complain("ERROR");
			break ;
	}
	return (0);
}
