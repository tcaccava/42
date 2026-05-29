#include "sed.hpp"

void if_open_file(std::ifstream &infile, const char* filename)
{
    infile.open(filename);
	if (!infile)
	{
		std::cerr << RED << "Error: cannot open input file.\n"
			<< RESET;
		std::exit(EXIT_FAILURE);
	}
}

void of_open_file(std::ofstream &outfile, const char* filename)
{
    outfile.open(filename);
	if (!outfile)
	{
		std::cerr << RED << "Error: cannot open output file.\n"
			<< RESET;
		std::exit(EXIT_FAILURE);
	}
}

void	check_args(int ac)
{
	if (ac != 4)
	{
		std::cout << RED << "INVALID NUMBER OF ARGUMENTS!" << RESET << std::endl;
		std::exit(EXIT_FAILURE);
	}
}
