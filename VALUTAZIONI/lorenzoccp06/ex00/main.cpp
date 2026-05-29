# include "ScalarConverter.hpp"
#include <sstream>
#include <iostream>
#include <limits>
#include <cerrno>
#include <iomanip>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <value>" << std::endl;
		return 1;
	}

	ScalarConverter::convert(argv[1]);

	return 0;

}