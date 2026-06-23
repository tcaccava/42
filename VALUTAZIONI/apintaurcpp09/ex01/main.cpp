#include "RPN.hpp"
#include <iostream>

int main(int argc, char* argv[]) {

	try {
		if (argc < 2) {
			throw std::runtime_error("Error: invalid cli arguments.");
		}

		Rpn			rpn;
		std::string input(argv[1]);

		rpn.compute(input);

		std::cout << rpn.getResult() << std::endl;

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
