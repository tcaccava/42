# include <cstdlib>
# include <ctime>
# include <iostream>
# include "A.hpp"
# include "B.hpp"
# include "C.hpp"

Base * generate(void) {
	// use implicit cast for promotion
	int random = rand() % 3;
	if (random == 0)
		return new A;
	else if (random == 1)
		return new B;
	else
		return new C;
}

// use dynamic_cast for safe downcasting
// https://www.geeksforgeeks.org/dynamic-_cast-in-cpp/

void identify(Base* p) {
	if (dynamic_cast<A*>(p) != NULL) {
		std::cout << "A" << std::endl;
		return;
	}
	if (dynamic_cast<B*>(p) != NULL) {
		std::cout << "B" << std::endl;
		return;
	}
	if (dynamic_cast<C*>(p) != NULL) {
		std::cout << "C" << std::endl;
		return;
	}
	std::cout << "Unknown type" << std::endl;
}

void  identify(Base& p) {
	try {
		dynamic_cast<A&>(p);
		std::cout << "A" << std::endl;
	} catch (std::exception& e) {
		try {
			dynamic_cast<B&>(p);
			std::cout << "B" << std::endl;
		} catch (std::exception& e) {
			try {
				dynamic_cast<C&>(p);
				std::cout << "C" << std::endl;
			} catch (std::exception& e) {
				std::cout << "Unknown type" << std::endl;
			}
		}
	}
}