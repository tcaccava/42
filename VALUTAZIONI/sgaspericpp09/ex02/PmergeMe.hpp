#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <iosfwd>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <ctime>
#include <cctype>
#include <cstddef>
#include <iterator>

// List functions
namespace listFunctions {
	std::list<int> strToList(std::string str);
	void fordJohnson(std::list<int> &lst);
	std::list<int> algoRun(std::string str);
}
// Vector functions
namespace vectorFunctions {
	std::vector<int> strTovector(std::string str);
	void fordJohnson(std::vector<int> &lst);
	std::vector<int> algoRun(std::string str);
}

int fordJohnson(std::list<int> lst, std::vector<int> vec);

template<typename T>
void print(T container, std::string when, double time_taken, std::string containerType) {
	if (when != "") {
		if (when == "before")
			std::cout << "Before: ";
		else if (when == "after")
			std::cout << "After: ";
	}
	for (typename T::iterator it = container.begin(); it != container.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	if (when == "after") {
		std::cout << "Time taken to process a range of " << container.size() << " elements with std::" << containerType << " : " << time_taken << " us" << std::endl;
	}
}

template <typename T>
std::string numToStr(T num) {
	std::ostringstream ss;
	ss << num;
	return ss.str();
}
#endif
