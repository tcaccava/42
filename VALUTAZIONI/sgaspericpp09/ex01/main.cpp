#include "RPN.hpp"

int	main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " \"RPN expression string\"" << std::endl;
		return 1;
	}
	try {
		RPN rpnCalculator(argv[1]);
		rpnCalculator.stackExec();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
