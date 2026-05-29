#include "PmergeMe.hpp"

int fordJohnson(std::list<int> lst, std::vector<int> vec) {
	print(lst, "before", 0, "list");
	print(vec, "before", 0, "vector");
	time_t start = clock();
	listFunctions::fordJohnson(lst);
	time_t end = clock();
	time_t start_vec = clock();
	vectorFunctions::fordJohnson(vec);
	time_t end_vec = clock();
	double time_taken_vec = double(end_vec - start_vec) / CLOCKS_PER_SEC * 1000000;
	double time_taken_lst = double(end - start) / CLOCKS_PER_SEC * 1000000;
	print(lst, "after", time_taken_lst, "list");
	print(vec, "after", time_taken_vec, "vector");
	return (0);
}