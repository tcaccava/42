#include "PmergeMe.hpp"
#include <cmath>
#include <vector>

// Utility functions for vectors
namespace vectorFunctions {
	std::vector<int> strTovector(std::string str) {
		std::vector<int> output;
		size_t strLen = str.length();
		while (strLen > 0) {
			if (str[0] == ' ') {
				str = str.substr(1);
				strLen = str.length();
				continue ;
			}
			if (str[0] < '0' || str[0] > '9') {
				std::cerr << "Error: Invalid character in input string" << std::endl;
				output.clear();
				return output;
			}
			std::string numStr = str.substr(0, str.find(' '));
			str = str.substr(numStr.length());
			std::istringstream iss(numStr);
			int num;
			if (!(iss >> num)) {
				std::cerr << "Error: Invalid number in input string" << std::endl;
				output.clear();
				return output;
			}
			if (std::find(output.begin(), output.end(), num) != output.end()) {
				std::cerr << "Error: Duplicate number in input string" << std::endl;
				output.clear();
				return output;
			}
			output.push_back(num);
			strLen = str.length();
		}
		return output;
	}

	int jacobsthalSequence(size_t n) {
		return round((pow(2, n + 1) + pow(-1, n)) / 3);
	}

	std::vector<int>::iterator currentEl(size_t length, std::vector<int> &lst) {
		std::vector<int>::iterator it = lst.begin();
		for (size_t i = 0; i < length; i++)
			++it;
		return it;
	}

	size_t findIndex(std::vector<int>::iterator it, std::vector<int> &lst) {
		size_t index = 0;
		for (std::vector<int>::iterator iter = lst.begin(); iter != it; ++iter) {
			++index;
		}
		return index;
	}

	void insert(std::vector<int> main, std::vector<int> pend, std::vector<int>::value_type odd, std::vector<int> left, std::vector<int> &lst, bool isOdd, size_t order) {
		std::vector<int>::iterator endMain;
		if (pend.size() == 1) {
			endMain = std::upper_bound(main.begin(), main.end(), *pend.begin());
			main.insert(endMain, *pend.begin());
		}
		else if (pend.size() > 1) {
			size_t jacSize = 3;
			size_t count = 0;
			size_t index;
			size_t decAmount;
			while (!pend.empty()) {
				index = jacobsthalSequence(jacSize ) - jacobsthalSequence(jacSize - 1);
				if (index > pend.size())
					index = pend.size();
				decAmount = 0;
				while (index) {
					endMain = main.begin();
					if (jacobsthalSequence(jacSize + count) - decAmount <= main.size()) {
						endMain = vectorFunctions::currentEl(jacobsthalSequence(jacSize + count) - decAmount, main);
					}
					else
						endMain = main.end();
					std::vector<int>::iterator itPend = pend.begin();
					for (size_t i = 0; i < index - 1; i++)
						++itPend;
					endMain = std::upper_bound(main.begin(), endMain, *itPend);
					main.insert(endMain, *itPend);
					pend.erase(itPend);
					index--;
					decAmount++;
					count++;
				}
				jacSize++;
			}
		}
		std::vector<int> yaslam; // temporary vector to hold sorted elements
		if (isOdd) {
			endMain = std::upper_bound(main.begin(), main.end(), odd);
			main.insert(endMain, odd);
		}
		for (std::vector<int>::iterator i = main.begin(); i != main.end(); i++) {
			std::vector<int>::iterator it = std::find(lst.begin(), lst.end(), *i);
			yaslam.insert(yaslam.end(), vectorFunctions::currentEl(vectorFunctions::findIndex(it, lst) - (order -1), lst), vectorFunctions::currentEl(vectorFunctions::findIndex(it, lst) + 1, lst));
			}
			yaslam.insert(yaslam.end(), left.begin(), left.end());
			lst = yaslam;
	}

	void fordJohnson(std::vector<int> &lst) {
		static size_t order = 1;
		size_t unit_size = lst.size() / order;
		if (unit_size < 2) return ;

		bool isOdd = unit_size % 2 == 1;
		std::vector<int>::iterator start = lst.begin();

		size_t steps = (order * unit_size) - (isOdd * order);
		std::vector<int>::iterator end = vectorFunctions::currentEl(steps, lst);
		//std::cout << "I — Merging " << "1. Split the vector into smaller pairs. The size of each pair is " << order << std::endl;
		for (size_t startIdx = vectorFunctions::findIndex(start, lst); startIdx < vectorFunctions::findIndex(end, lst); startIdx += (order * 2)) {
			size_t it1 = startIdx + (order - 1); // *(it + (order - 1))
			size_t it2 = startIdx + ((order * 2) - 1); //
			it1 = *vectorFunctions::currentEl(it1, lst);
			it2 = *vectorFunctions::currentEl(it2, lst);

			//std::cout << "I — Merging " << "2. Sort each pair individually using insertion sort." << it1 << " and " << it2 << std::endl;
			if (it1 > it2) {
				for (size_t i = 0; i < order; i++) {
					std::vector<int>::iterator it_temp1 = vectorFunctions::currentEl	(startIdx + i, lst); // *(it + i)
					std::vector<int>::iterator it_temp2 = vectorFunctions::currentEl	(startIdx + order + i, lst); // *(it + order + i)
					std::swap(*it_temp1, *it_temp2);
				}
			}
		}
		//std::cout << "I — Merging " << "3. Repeat with increasing order multiplying by 2 each time." << std::endl;
		order *= 2;
		fordJohnson(lst);
		order /= 2;
		//std::cout << "II — Inserting " << "1. Generating pend and main." << std::endl;
		std::vector<int> main;
		std::vector<int> pend;
		std::vector<int>::value_type odd = 0;
		std::vector<int> left;
		main.push_back(*vectorFunctions::currentEl(vectorFunctions::findIndex(start, lst) + order - 1, lst)); // a1
		main.push_back(*vectorFunctions::currentEl(vectorFunctions::findIndex(start, lst) + order * 2 - 1, lst));
		//std::cout << "II — Inserting " << "Adding " << *vectorFunctions::currentEl(vectorFunctions::findIndex(start, lst) + order - 1, lst) << " and " << *vectorFunctions::currentEl(vectorFunctions::findIndex(start, lst) + order * 2 - 1, lst) << " to main." << std::endl;
		for (size_t iter = vectorFunctions::findIndex(start, lst) + order * 2; iter < vectorFunctions::findIndex(end, lst); iter += order) {
			pend.push_back(*vectorFunctions::currentEl(iter + order - 1, lst));
			iter += order;
			main.push_back(*vectorFunctions::currentEl(iter + order - 1, lst));
		}
		// Push elements from pend to main and vice versa
		if (isOdd) odd = *vectorFunctions::currentEl(vectorFunctions::findIndex(end, lst) + order - 1, lst);

		// Save leftover elements that cannot form pairs on their own
		left.insert(left.end(), vectorFunctions::currentEl(vectorFunctions::findIndex(end, lst) + (order * isOdd), lst), lst.end());
		//std::cout << "II — Inserting " << "2. Sort the pend pairs into the main set based on the last element of each pair. We use binary search, optimized with the Jacobsthal sequence, and repeat the process while dividing the order (pair size) by 2 each time." << std::endl;
		if (isOdd || !pend.empty()) {
			//std::cout << "II — Inserting " << "There are elements to insert from pend into main: ";
			for (std::vector<int>::iterator it = pend.begin(); it != pend.end(); ++it) {
				//std::cout << *it << " ";
			}
			//std::cout << std::endl;
			vectorFunctions::insert(main, pend, odd, left, lst, isOdd, order);
		//std::cout << "II — Inserting " << "3. Finally, insert any remaining elements from pend into main using binary search." << std::endl;
		}
	}
	std::vector<int> algoRun(std::string str) {
		std::vector<int> vec = vectorFunctions::strTovector(str);
		if (vec.empty()) {
			return vec;
		}
		try {
			print(vec, "before", 0, "vector");
			time_t start = clock();
			vectorFunctions::fordJohnson(vec);
			time_t end = clock();
			double time_taken = double(end - start) / CLOCKS_PER_SEC * 1000000;
			print(vec, "after", time_taken, "vector");
		} catch (const std::exception &e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return vec;
		}
		return vec;
	}
}
