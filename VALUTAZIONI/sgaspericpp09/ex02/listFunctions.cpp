#include "PmergeMe.hpp"
#include <cmath>
#include <ctime>
#include <list>

// Utility functions for lists
namespace listFunctions {
	std::list<int> strToList(std::string str) {
		std::list<int> output;
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

	std::list<int>::iterator currentEl(size_t length, std::list<int> &lst) {
		std::list<int>::iterator it = lst.begin();
		for (size_t i = 0; i < length; i++)
			++it;
		return it;
	}

	size_t findIndex(std::list<int>::iterator it, std::list<int> &lst) {
		size_t index = 0;
		for (std::list<int>::iterator iter = lst.begin(); iter != it; ++iter) {
			++index;
		}
		return index;
	}

	void insert(std::list<int> main, std::list<int> pend, std::list<int>::value_type odd, std::list<int> left, std::list<int> &lst, bool isOdd, size_t order) {
		std::list<int>::iterator endMain;
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
						endMain = listFunctions::currentEl(jacobsthalSequence(jacSize + count) - decAmount, main);
					}
					else
						endMain = main.end();
					std::list<int>::iterator itPend = pend.begin();
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
		std::list<int> yaslam; // temporary list to hold sorted elements
		if (isOdd) {
			endMain = std::upper_bound(main.begin(), main.end(), odd);
			main.insert(endMain, odd);
		}
		for (std::list<int>::iterator i = main.begin(); i != main.end(); i++) {
			std::list<int>::iterator it = std::find(lst.begin(), lst.end(), *i);
			yaslam.insert(yaslam.end(), listFunctions::currentEl(listFunctions::findIndex(it, lst) - (order -1), lst), listFunctions::currentEl(listFunctions::findIndex(it, lst) + 1, lst));
			}
			yaslam.insert(yaslam.end(), left.begin(), left.end());
			lst = yaslam;
	}

	void fordJohnson(std::list<int> &lst) {
		static size_t order = 1;
		size_t unit_size = lst.size() / order;
		if (unit_size < 2) return ;

		bool isOdd = unit_size % 2 == 1;
		std::list<int>::iterator start = lst.begin();

		size_t steps = (order * unit_size) - (isOdd * order);
		std::list<int>::iterator end = listFunctions::currentEl(steps, lst);
		//std::cout << "I — Merging " << "1. Split the list into smaller pairs. The size of each pair is " << order << std::endl;
		for (size_t startIdx = listFunctions::findIndex(start, lst); startIdx < listFunctions::findIndex(end, lst); startIdx += (order * 2)) {
			size_t it1 = startIdx + (order - 1); // *(it + (order - 1))
			size_t it2 = startIdx + ((order * 2) - 1); //
			it1 = *listFunctions::currentEl(it1, lst);
			it2 = *listFunctions::currentEl(it2, lst);

			//std::cout << "I — Merging " << "2. Sort each pair individually using insertion sort." << it1 << " and " << it2 << std::endl;
			if (it1 > it2) {
				for (size_t i = 0; i < order; i++) {
					std::list<int>::iterator it_temp1 = listFunctions::currentEl	(startIdx + i, lst); // *(it + i)
					std::list<int>::iterator it_temp2 = listFunctions::currentEl	(startIdx + order + i, lst); // *(it + order + i)
					std::swap(*it_temp1, *it_temp2);
				}
			}
		}
		//std::cout << "I — Merging " << "3. Repeat with increasing order multiplying by 2 each time." << std::endl;
		order *= 2;
		fordJohnson(lst);
		order /= 2;
		//std::cout << "II — Inserting " << "1. Generating pend and main." << std::endl;
		std::list<int> main;
		std::list<int> pend;
		std::list<int>::value_type odd = 0;
		std::list<int> left;
		main.push_back(*listFunctions::currentEl(listFunctions::findIndex(start, lst) + order - 1, lst)); // a1
		main.push_back(*listFunctions::currentEl(listFunctions::findIndex(start, lst) + order * 2 - 1, lst));
		//std::cout << "II — Inserting " << "Adding " << *listFunctions::currentEl(listFunctions::findIndex(start, lst) + order - 1, lst) << " and " << *listFunctions::currentEl(listFunctions::findIndex(start, lst) + order * 2 - 1, lst) << " to main." << std::endl;
		for (size_t iter = listFunctions::findIndex(start, lst) + order * 2; iter < listFunctions::findIndex(end, lst); iter += order) {
			pend.push_back(*listFunctions::currentEl(iter + order - 1, lst));
			iter += order;
			main.push_back(*listFunctions::currentEl(iter + order - 1, lst));
		}
		// Push elements from pend to main and vice versa
		if (isOdd) odd = *listFunctions::currentEl(listFunctions::findIndex(end, lst) + order - 1, lst);

		// Save leftover elements that cannot form pairs on their own
		left.insert(left.end(), listFunctions::currentEl(listFunctions::findIndex(end, lst) + (order * isOdd), lst), lst.end());
		//std::cout << "II — Inserting " << "2. Sort the pend pairs into the main set based on the last element of each pair. We use binary search, optimized with the Jacobsthal sequence, and repeat the process while dividing the order (pair size) by 2 each time." << std::endl;
		if (isOdd || !pend.empty()) {
			//std::cout << "II — Inserting " << "There are elements to insert from pend into main: ";
			for (std::list<int>::iterator it = pend.begin(); it != pend.end(); ++it) {
				//std::cout << *it << " ";
			}
			//std::cout << std::endl;
			listFunctions::insert(main, pend, odd, left, lst, isOdd, order);
		//std::cout << "II — Inserting " << "3. Finally, insert any remaining elements from pend into main using binary search." << std::endl;
		}
	}
	std::list<int> algoRun(std::string str) {
		std::list<int> lst = listFunctions::strToList(str);
		if (lst.empty()) {
			return lst;
		}
		try {
			print(lst, "before", 0, "list");
			time_t start = clock();
			listFunctions::fordJohnson(lst);
			time_t end = clock();
			double time_taken = double(end - start) / CLOCKS_PER_SEC * 1000000;
			print(lst, "after", time_taken, "list");
		} catch (const std::exception &e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return lst;
		}
		return lst;
	}
}
