#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <utility>
#include <sys/time.h>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) : _vec(other._vec), _deq(other._deq) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		_vec = other._vec;
		_deq = other._deq;
	}
	return (*this);
}

PmergeMe::~PmergeMe() {}

// ----------------------------- Parsing ------------------------------------

static bool	isPositiveInt(const std::string& s, long& out)
{
	if (s.empty())
		return (false);
	for (std::string::size_type i = 0; i < s.size(); ++i)
		if (!std::isdigit(static_cast<unsigned char>(s[i])))
			return (false);
	std::istringstream	iss(s);
	long	v;
	iss >> v;
	if (iss.fail() || v < 0 || v > 2147483647L)
		return (false);
	out = v;
	return (true);
}

void	PmergeMe::parse(int argc, char** argv)
{
	if (argc < 2)
		throw std::runtime_error("no input");
	for (int i = 1; i < argc; ++i)
	{
		long	v;
		if (!isPositiveInt(argv[i], v))
			throw std::runtime_error("invalid input");
		_vec.push_back(static_cast<int>(v));
		_deq.push_back(static_cast<int>(v));
	}
}

// ------------------------ Jacobsthal helper -------------------------------

// J(n) = J(n-1) + 2*J(n-2) ; J(0)=0, J(1)=1 -> 0,1,1,3,5,11,21,43,...
// (Jacobsthal sequence, drives the binary-insertion order)
static long	jacobsthal(int n)
{
	if (n == 0)
		return (0);
	if (n == 1)
		return (1);
	long	a = 0, b = 1, c = 1;
	for (int i = 2; i <= n; ++i)
	{
		c = b + 2 * a;
		a = b;
		b = c;
	}
	return (c);
}

// ====================== Ford-Johnson : std::vector ========================

// Finds the first index in [0,hi) with base[idx] >= value (manual lower_bound).
static size_t	lowerBoundVec(const std::vector<int>& base, size_t hi, int value)
{
	size_t	lo = 0;
	while (lo < hi)
	{
		size_t	mid = lo + (hi - lo) / 2;
		if (base[mid] < value)
			lo = mid + 1;
		else
			hi = mid;
	}
	return (lo);
}

// Inserts value into main keeping it sorted, searching only in [0,hi).
static void	binaryInsertVec(std::vector<int>& main, int value, size_t hi)
{
	size_t	pos = lowerBoundVec(main, hi, value);
	main.insert(main.begin() + pos, value);
}

static void	fordJohnsonVec(std::vector<int>& arr)
{
	size_t	n = arr.size();
	if (n < 2)
		return ;

	bool	hasStraggler = (n % 2 == 1);
	int		straggler = hasStraggler ? arr[n - 1] : 0;

	// 1) pairs (big, small)
	std::vector<std::pair<int, int> > pairs;
	for (size_t i = 0; i + 1 < n; i += 2)
	{
		int	a = arr[i];
		int	b = arr[i + 1];
		if (a < b)
			std::swap(a, b);
		pairs.push_back(std::make_pair(a, b));
	}

	// 2) recursively sort the "bigs"
	std::vector<int>	bigs;
	for (size_t i = 0; i < pairs.size(); ++i)
		bigs.push_back(pairs[i].first);
	fordJohnsonVec(bigs);

	// realign the pairs to the order of the sorted bigs (duplicate-safe)
	std::vector<std::pair<int, int> >	sorted;
	std::vector<bool>					used(pairs.size(), false);
	for (size_t i = 0; i < bigs.size(); ++i)
		for (size_t j = 0; j < pairs.size(); ++j)
			if (!used[j] && pairs[j].first == bigs[i])
			{
				used[j] = true;
				sorted.push_back(pairs[j]);
				break ;
			}

	// 3) main chain = b1 + all the bigs (already sorted)
	std::vector<int>	main;
	main.push_back(sorted[0].second);
	for (size_t i = 0; i < sorted.size(); ++i)
		main.push_back(sorted[i].first);

	// 4) insert the remaining smalls in Jacobsthal order
	size_t	pendCount = sorted.size() - 1; // the smalls of pairs 1..k-1
	std::vector<size_t>	order;
	long	prev = 1;
	for (int k = 3; ; ++k)
	{
		long	j = jacobsthal(k);
		if (j > static_cast<long>(pendCount))
			j = static_cast<long>(pendCount);
		for (long idx = j; idx > prev; --idx)
			order.push_back(static_cast<size_t>(idx));
		if (j >= static_cast<long>(pendCount))
			break ;
		prev = j;
	}
	if (pendCount >= 1)
		order.insert(order.begin(), 1); // b2 is always inserted first

	for (size_t t = 0; t < order.size(); ++t)
	{
		size_t	pairIdx = order[t];           // in range 1..pendCount
		int		value = sorted[pairIdx].second;
		int		partner = sorted[pairIdx].first;
		size_t	hi = lowerBoundVec(main, main.size(), partner);
		binaryInsertVec(main, value, hi);
	}
	if (hasStraggler)
		binaryInsertVec(main, straggler, main.size());

	arr = main;
}

// ======================= Ford-Johnson : std::deque ========================

static size_t	lowerBoundDeq(const std::deque<int>& base, size_t hi, int value)
{
	size_t	lo = 0;
	while (lo < hi)
	{
		size_t	mid = lo + (hi - lo) / 2;
		if (base[mid] < value)
			lo = mid + 1;
		else
			hi = mid;
	}
	return (lo);
}

static void	binaryInsertDeq(std::deque<int>& main, int value, size_t hi)
{
	size_t	pos = lowerBoundDeq(main, hi, value);
	main.insert(main.begin() + pos, value);
}

static void	fordJohnsonDeq(std::deque<int>& arr)
{
	size_t	n = arr.size();
	if (n < 2)
		return ;

	bool	hasStraggler = (n % 2 == 1);
	int		straggler = hasStraggler ? arr[n - 1] : 0;

	std::deque<std::pair<int, int> >	pairs;
	for (size_t i = 0; i + 1 < n; i += 2)
	{
		int	a = arr[i];
		int	b = arr[i + 1];
		if (a < b)
			std::swap(a, b);
		pairs.push_back(std::make_pair(a, b));
	}

	std::deque<int>	bigs;
	for (size_t i = 0; i < pairs.size(); ++i)
		bigs.push_back(pairs[i].first);
	fordJohnsonDeq(bigs);

	std::deque<std::pair<int, int> >	sorted;
	std::deque<bool>					used(pairs.size(), false);
	for (size_t i = 0; i < bigs.size(); ++i)
		for (size_t j = 0; j < pairs.size(); ++j)
			if (!used[j] && pairs[j].first == bigs[i])
			{
				used[j] = true;
				sorted.push_back(pairs[j]);
				break ;
			}

	std::deque<int>	main;
	main.push_back(sorted[0].second);
	for (size_t i = 0; i < sorted.size(); ++i)
		main.push_back(sorted[i].first);

	size_t	pendCount = sorted.size() - 1;
	std::deque<size_t>	order;
	long	prev = 1;
	for (int k = 3; ; ++k)
	{
		long	j = jacobsthal(k);
		if (j > static_cast<long>(pendCount))
			j = static_cast<long>(pendCount);
		for (long idx = j; idx > prev; --idx)
			order.push_back(static_cast<size_t>(idx));
		if (j >= static_cast<long>(pendCount))
			break ;
		prev = j;
	}
	if (pendCount >= 1)
		order.push_front(1);

	for (size_t t = 0; t < order.size(); ++t)
	{
		size_t	pairIdx = order[t];
		int		value = sorted[pairIdx].second;
		int		partner = sorted[pairIdx].first;
		size_t	hi = lowerBoundDeq(main, main.size(), partner);
		binaryInsertDeq(main, value, hi);
	}
	if (hasStraggler)
		binaryInsertDeq(main, straggler, main.size());

	arr = main;
}

// ------------------------------ Driver ------------------------------------

static double	nowMicroseconds()
{
	struct timeval	tv;
	gettimeofday(&tv, 0);
	return (static_cast<double>(tv.tv_sec) * 1000000.0
		+ static_cast<double>(tv.tv_usec));
}

void	PmergeMe::sortVector()
{
	fordJohnsonVec(_vec);
}

void	PmergeMe::sortDeque()
{
	fordJohnsonDeq(_deq);
}

void	PmergeMe::printSequence(const std::string& label,
			const std::vector<int>& seq) const
{
	std::cout << label;
	for (size_t i = 0; i < seq.size(); ++i)
		std::cout << " " << seq[i];
	std::cout << std::endl;
}

void	PmergeMe::run()
{
	printSequence("Before:", _vec);

	double	startV = nowMicroseconds();
	sortVector();
	double	endV = nowMicroseconds();

	double	startD = nowMicroseconds();
	sortDeque();
	double	endD = nowMicroseconds();

	printSequence("After: ", _vec);

	std::cout << "Time to process a range of " << _vec.size()
		<< " elements with std::vector : " << (endV - startV) << " us" << std::endl;
	std::cout << "Time to process a range of " << _deq.size()
		<< " elements with std::deque  : " << (endD - startD) << " us" << std::endl;
}
