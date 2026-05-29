#include "PmergeMe.hpp"
#include <ctime>

int main(int ac, char **av) {
	std::list<int> lst;
	std::vector<int> vec;
	if (ac < 2) {
		std::cout << "Error" << std::endl;
		return 1;
	}
	else if (ac == 2) {
		lst = listFunctions::strToList(av[1]);
		vec = vectorFunctions::strTovector(av[1]);
	}
	else if (ac > 2) {
		std::string str;
		for (int i = 1; i < ac; i++) {
			str += av[i];
			if (i != ac - 1)
				str += " ";
		}
		lst = listFunctions::strToList(str);
		vec = vectorFunctions::strTovector(str);
	}
	// List ordering
	try {
		fordJohnson(lst, vec);
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
