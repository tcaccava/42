#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

static int check_element(char *element)
{
	if (std::strlen(element) == 0 || std::strlen(element) > 10)
		return 1;
	if (std::strcmp(element, "0") == 0)
		return 1;
	if (std::strlen(element) == 10 && std::strcmp(element, "2147483647") > 0)
		return 1;

	for (size_t i = 0; element[i] != '\0'; ++i)
	{
		if (element[i] < '0' || element[i] > '9')
			return 1;
	}
	return 0;
}

std::vector<int> PmergeMe::check_args(char **args)
{
	std::vector<int> temp;

	while (*args != NULL)
	{
		if (check_element(*args))
			throw std::exception();

		int val = std::atoi(*args);

		bool duplicate = false;
		for (size_t i = 0; i < temp.size(); ++i)
		{
			if (temp[i] == val)
			{
				duplicate = true;
				break;
			}
		}

		if (duplicate)
			throw std::exception();

		temp.push_back(val);
		args++;
	}
	return temp;
}


void PmergeMe::insert_vector(char **args, std::vector<int> &vector)
{
	while (*args != NULL)
	{
		vector.push_back(std::atoi(*args));
		args++;
	}
}

void PmergeMe::insert_deque(char **args, std::deque<int> &deque)
{
	while (*args != NULL)
	{
		deque.push_back(std::atoi(*args));
		args++;
	}
}

void PmergeMe::gen_jacobsthal_order(std::vector<size_t> &order, size_t totalPairs)
{
	order.clear();
	if (totalPairs == 0) return;

	std::vector<size_t> J;
	J.push_back(0);
	J.push_back(1);
	while (J.back() < totalPairs)
		J.push_back(J[J.size() - 1] + 2 * J[J.size() - 2]);

	size_t prev = 1;
	for (size_t i = 2; i < J.size(); ++i)
	{
		size_t curr = J[i];
		if (curr > totalPairs) curr = totalPairs;

		for (size_t j = curr; j-- > prev;)
			order.push_back(j);

		prev = curr;
	}

	if (prev < totalPairs)
	{
		for (size_t j = totalPairs; j-- > prev;)
			order.push_back(j);
	}
}
