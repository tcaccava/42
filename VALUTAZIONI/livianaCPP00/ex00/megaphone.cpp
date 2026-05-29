#include <iostream>
#include <string>

int	main(int ac, char **av)
{
	int	arg;
	int	index;

	if (ac == 1)
	{
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
		return 0;
	}
	for (arg = 1; av[arg]; arg++)
	{
		for (index = 0; av[arg][index]; index++)
			std::cout << (char)std::toupper(av[arg][index]) ;
	}
	std::cout << std::endl ;
	return 0;
}

