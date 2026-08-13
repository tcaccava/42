#include "Replace.hpp"

int	main(int c, char **v)
{
	if (c != 4)
	{
		std::cerr << "Three arguments required" << std::endl;
		return (1);
	}

	if (!replaceInFile(v[1], v[2], v[3]))
		return (1);

	return (0);
}