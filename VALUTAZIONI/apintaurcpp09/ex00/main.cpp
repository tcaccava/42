#include "BitcoinExchange.hpp"
#include <iostream>

int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cerr << "Error: could not open a file." << std::endl;
		return (-1);
	}

	try {
		BitcoinExchange b;
		(void)argv;
		b.load(argv[1]);
		// b.print();
	} catch (const BitcoinExchange::BadFile& e) {
		std::cout << "Error: exception caught: " << e.what() << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Error: exception caught: " << e.what() << std::endl;
	}
}
