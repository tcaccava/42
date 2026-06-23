#include "PmergeMe.hpp"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <set>
#include <sstream>
#include <errno.h>
#include <limits>

PmergeMe::PmergeMe() : vec(), lst(), vecTime(0.0), listTime(0.0) {
}

PmergeMe::PmergeMe(const std::string& input)
	: vec(), lst(), vecTime(0.0), listTime(0.0) {

	if (input.empty())
		throw std::runtime_error("Error: input must be non-empty.");
	std::istringstream iss(input);
	std::string		   strValue;
	std::set<int>	   seen;
	char*			   endptr;
	long			   value;

	while (iss >> strValue) {
		errno = 0;
		value = std::strtol(strValue.c_str(), &endptr, 10);
		if (endptr == strValue.c_str() || *endptr != '\0' || errno == ERANGE
			|| value < 0 || value > std::numeric_limits<int>::max()) {
			throw std::runtime_error("Error: invalid input value: " + strValue);
		} else if (seen.find(value) == seen.end()) {
			seen.insert(static_cast<int>(value));
			vec.push_back(static_cast<int>(value));
			lst.push_back(static_cast<int>(value));
		}
	}
}

PmergeMe::PmergeMe(const PmergeMe& p)
	: vec(p.vec), lst(p.lst), vecTime(p.vecTime), listTime(p.listTime) {
}

PmergeMe& PmergeMe::operator=(const PmergeMe& p) {
	if (this != &p) {
		vec		 = p.vec;
		lst		 = p.lst;
		vecTime	 = p.vecTime;
		listTime = p.listTime;
	}
	return (*this);
}

void PmergeMe::sortVector() {
	clock_t start = clock();

	bool isSorted = true;
	for (size_t i = 0; i + 1 < vec.size(); ++i) {
		if (vec[i] > vec[i + 1]) {
			isSorted = false;
			break;
		}
	}
	if (isSorted) {
		vecTime = 0.0;
		return;
	}

	size_t size = vec.size();
	if (size <= 1) {
		vecTime = 0.0;
		return;
	}

	// Step 1: Create pairs of (max, min)
	std::vector<std::pair<int, int> > pairs;
	pairs.reserve(size / 2);

	for (size_t i = 0; i + 1 < size; i += 2) {
		if (vec[i] < vec[i + 1])
			pairs.push_back(std::make_pair(vec[i + 1], vec[i]));
		else
			pairs.push_back(std::make_pair(vec[i], vec[i + 1]));
	}

	// Step 2: Sort pairs by their max values (first element)
	std::sort(pairs.begin(), pairs.end());

	// Step 3: Build main chain from max elements
	std::vector<int> mainChain;
	mainChain.reserve(size);
	for (size_t i = 0; i < pairs.size(); i++) {
		mainChain.push_back(pairs[i].first);
	}

	// Step 4: Collect min elements
	std::vector<int> mins;
	mins.reserve(pairs.size());
	for (size_t i = 0; i < pairs.size(); i++) {
		mins.push_back(pairs[i].second);
	}

	// Step 5: Generate Jacobsthal indices for insertion order
	std::vector<int> jacobIndices;
	jacobIndices.reserve(mins.size());
	std::vector<char> usedIndices(mins.size(), 0);

	int k = 0;
	while (jacobIndices.size() < mins.size() && k < 64) {
		int j = 0;

		if (k % 2 == 0) {
			j = ((1 << (k + 1)) + 1) / 3;
		} else {
			j = ((1 << (k + 1)) - 1) / 3;
		}
		if (j > 0 && static_cast<size_t>(j) <= mins.size()) {
			int idx = j - 1;
			if (!usedIndices[idx]) {
				jacobIndices.push_back(idx);
				usedIndices[idx] = 1;
			}
		}
		k++;
	}

	// Fill remaining indices if any
	for (size_t i = 0; jacobIndices.size() < mins.size() && i < mins.size();
		 i++) {
		if (!usedIndices[i]) {
			jacobIndices.push_back(i);
			usedIndices[i] = 1;
		}
	}

	// Step 6: Insert min elements into main chain in Jacobsthal order
	for (size_t i = 0; i < jacobIndices.size(); i++) {
		int						   val = mins[jacobIndices[i]];
		std::vector<int>::iterator pos =
			std::lower_bound(mainChain.begin(), mainChain.end(), val);
		mainChain.insert(pos, val);
	}

	// Step 7: Insert straggler if exists
	if (size % 2 != 0) {
		int						   straggler = vec.back();
		std::vector<int>::iterator pos =
			std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(pos, straggler);
	}

	vec = mainChain;

	clock_t end = clock();
	vecTime		= static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
}

void PmergeMe::sortList() {
	clock_t startTime = clock();

	bool isSorted = true;
	for (std::list<int>::iterator it = lst.begin(), next = ++lst.begin();
		 next != lst.end(); ++it, ++next) {
		if (*it > *next) {
			isSorted = false;
			break;
		}
	}
	if (isSorted) {
		listTime = 0.0;
		return;
	}

	size_t size = lst.size();
	if (size <= 1) {
		listTime = 0.0;
		return;
	}

	// Step 1: Create pairs of (max, min)
	std::list<std::pair<int, int> > pairs;
	std::list<int>::iterator	   it  = lst.begin();
	std::list<int>::iterator	   end = lst.end();

	while (it != end) {
		int firstVal = *it;
		it++;
		if (it == end) {
			break;
		}
		int secondVal = *it;
		it++;

		if (firstVal < secondVal) {
			pairs.push_back(std::make_pair(secondVal, firstVal));
		} else {
			pairs.push_back(std::make_pair(firstVal, secondVal));
		}
	}

	// Step 2: Sort pairs by their max values (first element)
	pairs.sort();

	// Step 3: Build main chain from max elements
	std::list<int> mainChain;
	for (std::list<std::pair<int, int> >::iterator pit = pairs.begin();
		 pit != pairs.end(); pit++) {
		mainChain.push_back(pit->first);
	}

	// Step 4: Collect min elements
	std::list<int> mins;
	for (std::list<std::pair<int, int> >::iterator pit = pairs.begin();
		 pit != pairs.end(); pit++) {
		mins.push_back(pit->second);
	}

	// Step 5: Generate Jacobsthal indices for insertion order
	std::vector<int> jacobIndices;
	jacobIndices.reserve(mins.size());
	std::vector<char> usedIndices(mins.size(), 0);

	int k = 0;
	while ((size_t)jacobIndices.size() < mins.size() && k < 64) {
		int j = 0;
		if ((k % 2) == 0) {
			j = ((1 << (k + 1)) + 1) / 3;
		} else {
			j = ((1 << (k + 1)) - 1) / 3;
		}
		if (j > 0 && static_cast<size_t>(j) <= mins.size()) {
			int idx = j - 1;
			if (usedIndices[idx] == 0) {
				jacobIndices.push_back(idx);
				usedIndices[idx] = 1;
			}
		}
		k++;
	}

	// Fill remaining indices if any
	for (size_t i = 0; jacobIndices.size() < mins.size() && i < mins.size();
		 i++) {
		if (usedIndices[i] == 0) {
			jacobIndices.push_back(i);
			usedIndices[i] = 1;
		}
	}

	// Step 6: Insert min elements into main chain in Jacobsthal order
	for (size_t i = 0; i < jacobIndices.size(); i++) {
		int val = 0;
		// get the min element at index jacobIndices[i]
		std::list<int>::iterator minIt = mins.begin();
		for (int idx = 0; idx < jacobIndices[i]; idx++) {
			minIt++;
		}
		val = *minIt;

		// find insertion position using lower_bound equivalent on list
		std::list<int>::iterator pos = mainChain.begin();
		while (pos != mainChain.end() && *pos < val) {
			pos++;
		}
		mainChain.insert(pos, val);
	}

	// Step 7: Insert straggler if exists
	if ((size % 2) != 0) {
		int						 straggler = lst.back();
		std::list<int>::iterator pos	   = mainChain.begin();
		while (pos != mainChain.end() && *pos < straggler) {
			pos++;
		}
		mainChain.insert(pos, straggler);
	}

	lst = mainChain;

	clock_t endTime = clock();
	listTime =
		static_cast<double>(endTime - startTime) * 1000000.0 / CLOCKS_PER_SEC;
}

void PmergeMe::setVector(const std::vector<int>& v) {
	if (v.empty()) {
		throw std::runtime_error("Error: cannot assign vector to "
								 "PmergeMe::vec. Vector must be non-empty");
	}

	std::vector<int> tmp;
	std::set<int>	 seen;

	for (size_t i = 0; i < v.size(); i++) {
		if (v.at(i) < 0) {
			throw std::runtime_error(
				"Error: cannot assign vector to PmergeMe::vec. Vector numbers "
				"must be non-negative");
		}
		if (seen.find(v.at(i)) == seen.end()) {
			tmp.push_back(v.at(i));
			seen.insert(v.at(i));
		}
	}
	vec = tmp;
}

void PmergeMe::setList(const std::list<int>& l) {
	if (l.empty()) {
		throw std::runtime_error("Error: cannot assign list to PmergeMe::lst. "
								 "List must be non-empty");
	}
	std::list<int>::const_iterator end = l.end();
	std::list<int>				   tmp;
	std::set<int>				   seen;

	for (std::list<int>::const_iterator i = l.begin(); i != end; i++) {
		if (*i < 0) {
			throw std::runtime_error(
				"Error: cannot assign list to PmergeMe::lst. List numbers must "
				"be non-negative");
		}
		if (seen.find(*i) == seen.end()) {
			tmp.push_back(*i);
			seen.insert(*i);
		}
	}
	lst = tmp;
}

void PmergeMe::printVector() {

	size_t size = vec.size();

	if (!size) {
		throw std::runtime_error("Error: cannot display empty containers.");
	}

	for (size_t i = 0; i < size; i++) {
		std::cout << vec.at(i) << " ";
	}
}

void PmergeMe::printList() {

	std::list<int>::const_iterator end = lst.end();

	if (lst.empty()) {
		throw std::runtime_error("Error: cannot display empty containers.");
	}

	for (std::list<int>::const_iterator i = lst.begin(); i != end; i++) {
		std::cout << *i << " ";
	}
}

void PmergeMe::reset() {
	vec.clear();
	lst.clear();
	vecTime = listTime = 0.0;
}

PmergeMe::~PmergeMe() {
}
