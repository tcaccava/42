#include "PmergeMe.hpp"

int main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "usage: ./PmergeMe [numbers...]" << std::endl;
		return 1;
	}
	try
	{
		PmergeMe pm;
		std::vector<int> check(pm.check_args(av + 1));

		std::cout << "Before : ";
		for (std::vector<int>::iterator it = check.begin(); it != check.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;

		// -------- vector --------
		std::vector<int> v;
		pm.insert_vector(av + 1, v);
		clock_t start_vector = clock();
		std::vector<int> vector_sorted = pm.merge(v);
		clock_t end_vector = clock();

		std::cout << "After : ";
		for (std::vector<int>::iterator it = vector_sorted.begin(); it != vector_sorted.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
		std::cout << "Time to process a range of " << v.size()
				<< " elements with std::vector : "
				<< static_cast<double>(end_vector - start_vector) / CLOCKS_PER_SEC * 1000
				<< " us" << std::endl;

		// -------- deque --------
		std::deque<int> d;
		pm.insert_deque(av + 1, d);
		clock_t start_deque = clock();
		std::deque<int> deque_sorted = pm.merge(d);
		clock_t end_deque = clock();

		std::cout << "Time to process a range of " << d.size()
				<< " elements with std::deque : "
				<< static_cast<double>(end_deque - start_deque) / CLOCKS_PER_SEC * 1000
				<< " us" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}
	return 0;
}
