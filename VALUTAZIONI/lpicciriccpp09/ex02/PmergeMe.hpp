#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <cstdlib>
#include <exception>
#include <cstring>
#include <algorithm>

class PmergeMe
{
public:
	PmergeMe();
	void insert_vector(char **args, std::vector<int> &vector);
	void insert_deque(char **args, std::deque<int> &deque);
	void gen_jacobsthal_order(std::vector<size_t> &order, size_t m);
	std::vector<int> check_args(char **args);

	template <typename Container>
	Container merge(const Container &input);
};

template <typename Container>
Container PmergeMe::merge(const Container &input)
{
	if (input.size() <= 1)
		return input;

	std::vector<std::pair<int, int> > pairs;
	typename Container::const_iterator it = input.begin();
	while (it != input.end())
	{
		int first = *it++;
		if (it == input.end())
		{
			pairs.push_back(std::make_pair(first, -1));
			break;
		}
		int second = *it++;
		if (first < second)
			pairs.push_back(std::make_pair(second, first));
		else
			pairs.push_back(std::make_pair(first, second));
	}

	bool hasLeftover = false;
	int leftover = 0;
	if (!pairs.empty() && pairs.back().second == -1)
	{
		hasLeftover = true;
		leftover = pairs.back().first;
		pairs.pop_back();
	}

	if (pairs.empty())
	{
		Container result;
		if (hasLeftover)
			result.push_back(leftover);
		return result;
	}

	Container bigs;
	for (size_t k = 0; k < pairs.size(); ++k)
		bigs.push_back(pairs[k].first);
	Container sortedBigs = merge(bigs);

	std::vector<std::pair<int, int> > sortedPairs;
	for (typename Container::iterator bigIt = sortedBigs.begin(); bigIt != sortedBigs.end(); ++bigIt)
	{
		for (size_t j = 0; j < pairs.size(); ++j)
		{
			if (pairs[j].first == *bigIt)
			{
				sortedPairs.push_back(pairs[j]);
				break;
			}
		}
	}

	Container sortedChain;
	sortedChain.push_back(sortedPairs[0].first);
	if (sortedPairs[0].second != -1)
	{
		typename Container::iterator pos =
			std::lower_bound(sortedChain.begin(), sortedChain.end(), sortedPairs[0].second);
		sortedChain.insert(pos, sortedPairs[0].second);
	}

	for (size_t i = 1; i < sortedPairs.size(); ++i)
		sortedChain.push_back(sortedPairs[i].first);

	for (size_t i = 1; i < sortedPairs.size(); ++i)
	{
		int small = sortedPairs[i].second;
		if (small != -1)
		{
			typename Container::iterator pos =
				std::lower_bound(sortedChain.begin(), sortedChain.end(), small);
			sortedChain.insert(pos, small);
		}
	}

	if (hasLeftover)
	{
		typename Container::iterator pos =
			std::lower_bound(sortedChain.begin(), sortedChain.end(), leftover);
		sortedChain.insert(pos, leftover);
	}

	return sortedChain;
}

#endif
