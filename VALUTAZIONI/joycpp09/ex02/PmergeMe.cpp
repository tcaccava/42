#include "PmergeMe.hpp"
#include <iterator>
#include <cstddef>
#include <algorithm>

int n_compares_vec;
int n_compares_list;

PmergeMe::PmergeMe(char **av)
{
	vec_debug_mode = false;
	list_debug_mode = false;
	int tmp;
	try {
		for (int i = 0; av[i]; i++)
		{
			std::string str(av[i]);
			if(str.empty())
				throw std::exception();
			if(str.find_first_not_of("0123456789") != std::string::npos)
				throw std::exception();
			tmp = atoi(av[i]);
			if(tmp < 0)
				throw std::exception();
			numbers_vec.push_back(tmp);
			numbers_list.push_back(tmp);
		}
	}
	catch (std::exception &e) { std::cout << "Error: Invalid input" << std::endl; exit(1);}
}

PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	//*this = other;
	this->numbers_vec = other.numbers_vec;
    this->numbers_list = other.numbers_list;
    this->list_rest = other.list_rest;
    this->vec_rest = other.vec_rest;
    this->vec_debug_mode = other.vec_debug_mode;
    this->list_debug_mode = other.list_debug_mode;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if(this != &other)
	{
		numbers_vec = other.numbers_vec;
		numbers_list = other.numbers_list;
		list_rest = other.list_rest;
		vec_rest = other.vec_rest;
		vec_debug_mode = other.vec_debug_mode;
		list_debug_mode = other.list_debug_mode;
	}
	return *this;
}

PmergeMe::PmergeMe()
{
}

void PmergeMe::set_vect_debug_mode(bool debug_mode)
{
	vec_debug_mode = debug_mode;
}

void PmergeMe::set_list_debug_mode(bool debug_mode)
{
	list_debug_mode = debug_mode;
}

void PmergeMe::merge_part_vector(vec_int &v, int vec_size, vec_vec_int &big_v)
{
	int size_of_v = 2 * vec_size;
	if (size_of_v == 0)
		size_of_v = 1;

	make_pairs_vec(v, size_of_v, big_v);
	if(vec_debug_mode)
		print_vec(big_v, vec_size);
	reorder_v(vec_size, big_v, v);

	if(big_v.size() > 3) 
		merge_part_vector(v, size_of_v, big_v);
	vect_insertion_part(big_v, vec_rest, vec_size, v); // Jacobstahal , binary search
}

void PmergeMe::make_pairs_vec(vec_int v, int size_of_v, vec_vec_int &big_v)
{
	if (size_of_v == 0)
		size_of_v = 1;
	big_v.clear();
	vec_rest.clear();
	vec_int small_v;
	for(size_t i = 0; i < v.size(); i += size_of_v)
	{
		if(i + size_of_v > v.size())
		{
			for(size_t j = i; j < v.size(); j++)
				vec_rest.push_back(v[j]);
			break;
		}
		small_v.clear();
		for(size_t j = i; j < i + size_of_v && j < v.size(); j++)
			small_v.push_back(v[j]);
		big_v.push_back(small_v);
	}
}

void PmergeMe::make_new_v(vec_vec_int &big_v, vec_int &v)
{
	v.clear();
	for(size_t i = 0; i < big_v.size(); i++)
		for (size_t j = 0; j < big_v[i].size(); j++)
			v.push_back(big_v[i][j]);
	for(size_t i = 0; i < vec_rest.size(); i++)
		v.push_back(vec_rest[i]);
}

void PmergeMe::make_new_list(list_list_int &big_v, list_int &v)
{
	v.clear();
	for(list_list_iter it = big_v.begin(); it != big_v.end(); it++)
		for(list_iter it2 = it->begin(); it2 != it->end(); it2++)
			v.push_back(*it2);
	for(list_iter it = list_rest.begin(); it != list_rest.end(); it++)
		v.push_back(*it);
}
void PmergeMe::reorder_v(int vec_size, vec_vec_int &big_v, vec_int &v)
{
	if(vec_size == 0)
		vec_size = 1;
	for(size_t i = 0; i + 1 < big_v.size(); i += 2)
	{
		n_compares_vec++;
		if (big_v[i].back() > big_v[i + 1].back())
			std::swap(big_v[i], big_v[i + 1]);
	}
	make_new_v(big_v, v);
}

void PmergeMe::reorder_list(int list_size, list_list_int &big_v, list_int &v)
{
	if(list_size == 0)
		list_size = 1;
	for(list_list_iter it = big_v.begin(); it != big_v.end();)
	{
		list_list_iter it2 = it;
		it2++;
		if(it2 == big_v.end())
			break;
		n_compares_list++;
		if (it->back() > it2->back())
			std::swap(*it, *it2);
		it++;
		it++;
	}
	make_new_list(big_v, v);
}

void PmergeMe::main_print_vec(vec_int v)
{
	for(vec_iter it = v.begin(); it != v.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}

void PmergeMe::main_print_list(list_int v)
{
	for(list_iter it = v.begin(); it != v.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}

unsigned long get_microseconds() 
{
	struct timeval Time;

	gettimeofday(&Time, NULL);
	unsigned long Micro = Time.tv_sec * 1000000 + Time.tv_usec;

	return Micro;
}

void PmergeMe::merge_insertion_sort_vector()
{
	vec_vec_int big_v;

	std::cout << "Before: ";
	size_t size = numbers_vec.size();
	main_print_vec(numbers_vec);

	unsigned long BeforeMicro = get_microseconds();
	merge_part_vector(numbers_vec, 0, big_v);
	unsigned long AfterMicro = get_microseconds();


	std::cout << "After: ";
	main_print_vec(numbers_vec);

	std::cout << "Time to process a range of " << size << " elements with std::vector : " << (AfterMicro - BeforeMicro) << " us" << std::endl;
	
	if(vec_debug_mode)
	{	
		std::cout << "number of comparisons: " << n_compares_vec << std::endl;
		is_sorted_vect(numbers_vec) ? std::cout << "sorted" << std::endl : std::cout << "not sorted" << std::endl;
		size != numbers_vec.size() ? std::cout << "size is wrong: " << size << std::endl : std::cout << "size is correct: " << size << std::endl;
	}
}

void PmergeMe::print_main_and_pend_chain_vect(vec_vec_int main_chain, vec_pair pend_chain, vec_int rest, int vec_size)
{
	for (int i = 0; i < vec_size; i++)
		std::cout << "\t";
	std::cout << "main_chain: ";
	for(vec_vec_iter it = main_chain.begin(); it != main_chain.end(); it++)
	{
		std::cout << "[";
		for(vec_iter it2 = it->begin(); it2 != it->end(); it2++)
		{
			std::cout << *it2;
			if(it2 + 1 != it->end())
				std::cout << " ";
		}
		std::cout << "]";
		if(it + 1 != main_chain.end())
			std::cout << " ";
	}

	std::cout << std::endl;
	for (int i = 0; i < vec_size; i++)
		std::cout << "\t";
	std::cout << "pend_chain: ";
	for(vec_pair_iter it = pend_chain.begin(); it != pend_chain.end(); it++)
	{
		std::cout << "[";
		for(vec_iter it2 = it->first.begin(); it2 != it->first.end(); it2++)
		{
			std::cout << *it2;
			if(it2 + 1 != it->first.end())
				std::cout << " ";
		}
		std::cout << "]";
		if(it + 1 != pend_chain.end())
			std::cout << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < vec_size; i++)
		std::cout << "\t";
	std::cout << "rest: ";
	for(vec_iter it = rest.begin(); it != rest.end(); it++)
	{
		std::cout << *it;
		if(it + 1 != rest.end())
			std::cout << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::print_main_and_pend_chain_list(list_list_int main_chain, list_pair pend_chain, list_int rest, int list_size)
{
	for (int i = 0; i < list_size; i++)
		std::cout << "\t";
	std::cout << "main_chain: ";
	for(list_list_iter it = main_chain.begin(); it != main_chain.end(); it++)
	{
		std::cout << "[";
		for(list_iter it2 = it->begin(); it2 != it->end(); it2++)
		{
			std::cout << *it2;
			if(advance_iter(it2, 1) != it->end())
				std::cout << " ";
		}
		std::cout << "]";
		if(advance_iter(it, 1) != main_chain.end())
			std::cout << " ";
	}

	std::cout << std::endl;
	for (int i = 0; i < list_size; i++)
		std::cout << "\t";
	std::cout << "pend_chain: ";
	for(list_pair_iter it = pend_chain.begin(); it != pend_chain.end(); it++)
	{
		std::cout << "[";
		for(list_iter it2 = it->first.begin(); it2 != it->first.end(); it2++)
		{
			std::cout << *it2;
			if(advance_iter(it2, 1) != it->first.end())
				std::cout << " ";
		}
		std::cout << "]";
		if(advance_iter(it, 1) != pend_chain.end())
			std::cout << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < list_size; i++)
		std::cout << "\t";
	std::cout << "rest: ";
	for(list_iter it = rest.begin(); it != rest.end(); it++)
	{
		std::cout << *it;
		if(advance_iter(it, 1) != rest.end())
			std::cout << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::print_vec(vec_vec_int big_v, int vec_size)
{
	vec_size *= 2;
	if(vec_size == 0)
		vec_size = 1;
	for (int i = 0; i < vec_size; i++)
		std::cout << "\t";
	std::cout << "vec_size: " << vec_size << std::endl;
	for (int i = 0; i < vec_size; i++)
		std::cout << "\t";
	std::cout << "numbers: ";
	for (size_t i = 0; i < numbers_vec.size(); i++)
	{
		std::cout << numbers_vec[i];
		if(i + 1 != numbers_vec.size())
			std::cout << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < vec_size; i++)
		std::cout << "\t";
	std::cout << "big_v: ";
	for(vec_vec_iter it = big_v.begin(); it != big_v.end(); it++)
	{
		std::cout << "[";
		for(vec_iter it2 = it->begin(); it2 != it->end(); it2++)
		{
			std::cout << *it2;
			if(it2 + 1 != it->end())
				std::cout << " ";
		}
		std::cout << "]";
		if(it + 1 != big_v.end())
			std::cout << " ";
	}

	if(!vec_rest.empty())
	{
		std::cout << " + [";
		for(vec_iter it = vec_rest.begin(); it != vec_rest.end(); it++)
		{
			std::cout << *it;
			if(it + 1 != vec_rest.end())
				std::cout << " ";
		}
		std::cout << "]";
	}
	std::cout << std::endl;
}

void PmergeMe::print_list(list_list_int big_v, int list_size)
{
	list_size *= 2;
	if(list_size == 0)
		list_size = 1;
	for (int i = 0; i < list_size; i++)
		std::cout << "\t";
	std::cout << "list_size: " << list_size << std::endl;
	for (int i = 0; i < list_size; i++)
		std::cout << "\t";
	std::cout << "numbers: ";
	for(list_iter it = numbers_list.begin(); it != numbers_list.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	for (int i = 0; i < list_size; i++)
		std::cout << "\t";
	std::cout << "big_v: ";
	for(list_list_iter it = big_v.begin(); it != big_v.end(); it++)
	{
		std::cout << "[";
		for(list_iter it2 = it->begin(); it2 != it->end(); it2++)
		{
			std::cout << *it2;
		if(advance_iter(it2, 1) != it->end())
			std::cout << " ";
		}
		std::cout << "]";
		if(advance_iter(it, 1) != big_v.end())
			std::cout << " ";
	}

	if(!list_rest.empty())
	{
		std::cout << " + [";
		for(list_iter it = list_rest.begin(); it != list_rest.end(); it++)
		{
			std::cout << *it;
			if(advance_iter(it, 1) != list_rest.end())
				std::cout << " ";
		}
		std::cout << "]";
	}
	std::cout << std::endl;
}

void PmergeMe::vect_insertion_part(vec_vec_int &big_v, vec_int rest, int vec_size, vec_int &v)
{
	if(vec_debug_mode)
		print_vec(big_v, vec_size);
	vec_vec_int main_chain;
	vec_pair pend_chain;

	pend_chain.reserve(big_v.size());
	main_chain.reserve(big_v.size());


	make_main_chain_and_pend_chain_vec(main_chain, pend_chain, big_v);

	if(vec_debug_mode)
		print_main_and_pend_chain_vect(main_chain, pend_chain, rest, vec_size);

    long long jacobsthal_diff[] = {
        2u, 2u, 6u, 10u, 22u, 42u, 86u, 170u, 342u, 682u, 1366u,
        2730u, 5462u, 10922u, 21846u, 43690u, 87382u, 174762u, 349526u, 699050u,
        1398102u, 2796202u, 5592406u, 11184810u, 22369622u, 44739242u, 89478486u,
        178956970u, 357913942u, 715827882u, 1431655766u, 2863311530u, 5726623062u,
        11453246122u, 22906492246u, 45812984490u, 91625968982u, 183251937962u,
        366503875926u, 733007751850u, 1466015503702u, 2932031007402u, 5864062014806u,
        11728124029610u, 23456248059222u, 46912496118442u, 93824992236886u, 187649984473770u,
        375299968947542u, 750599937895082u, 1501199875790165u, 3002399751580331u,
        6004799503160661u, 12009599006321322u, 24019198012642644u, 48038396025285288u,
        96076792050570576u, 192153584101141152u, 384307168202282304u, 768614336404564608u,
        1537228672809129216u, 3074457345618258432u, 6148914691236516864u
    };
	
	size_t i = 0;
	while(pend_chain.size())
	{
		vec_pair_iter end = pend_chain.begin();

		while (jacobsthal_diff[i] > 1) {
			end++;
			jacobsthal_diff[i]--;
		}
		i++;
		if(end >= pend_chain.end())
			end = pend_chain.end() - 1;
		while(true)
		{
			vec_vec_iter it2 = std::lower_bound(main_chain.begin(), end->second, end->first, comp_func_vec);
			main_chain.insert(it2, end->first);
			update_pend_chain_vec(pend_chain, end->second);
			pend_chain.erase(end);

			if(end == pend_chain.begin())
				break;
			end--;
		}
	}

	make_v_from_main_and_pend_chain(main_chain, rest, v);
	
	make_pairs_vec(v, vec_size, big_v);
}

void PmergeMe::make_main_chain_and_pend_chain_vec(vec_vec_int &main_chain, vec_pair &pend_chain, vec_vec_int big_v)
{
	size_t i = 0;

	for(; i < 2 && i < big_v.size(); i++)
	{
		main_chain.push_back(big_v[i]);
	}

	std::pair<vec_int, vec_vec_iter > pair_temp;
	for(vec_vec_iter it = big_v.begin() + 2; i == 2 && it != big_v.end(); )
	{
		pair_temp.first = *it;
		pair_temp.second = main_chain.end();
		++it;
		if (it != big_v.end())
		{
			pair_temp.second = main_chain.insert(main_chain.end(), *it);
			++it;
		}
		pend_chain.push_back(pair_temp);
	}
}

void PmergeMe::make_main_chain_and_pend_chain_list(list_list_int &main_chain, list_pair &pend_chain, list_list_int big_v)
{
	size_t i = 0;
	list_list_iter it2 = big_v.begin();
	for(;i < 2 && it2 != big_v.end();)
	{
		main_chain.push_back(*it2);
		i++;
		it2++;
	}


	std::pair<list_int, list_list_iter > pair_temp;
	for(list_list_iter it = it2; i == 2 && it != big_v.end(); )
	{
		pair_temp.first = *it;
		pair_temp.second = main_chain.end();
		++it;
		if (it != big_v.end())
		{
			pair_temp.second = main_chain.insert(main_chain.end(), *it);
			++it;
		}
		pend_chain.push_back(pair_temp);
	}
}

void PmergeMe::update_pend_chain_vec(vec_pair &pend_chain, vec_vec_iter it)
{
	for(vec_pair_iter it2 = pend_chain.begin(); it2 != pend_chain.end(); it2++)
	{
		if(it2->second != it)
			it2->second++;
	}
}

bool comp_func_vec(vec_int v1, vec_int v2)
{
	n_compares_vec++;
	return v1.back() < v2.back();
}

bool comp_func_list(list_int v1, list_int v2)
{
	n_compares_list++;
	return v1.back() < v2.back();
}
void PmergeMe::split_big_v(vec_vec_int &big_v, vec_int v, int vec_size)
{
	if(vec_size == 0)
		vec_size = 1;
	big_v.clear();
	vec_int small_v;
	for(size_t i = 0; i < v.size(); i += vec_size)
	{
		small_v.clear();
		small_v.push_back(v[i]);
		if(i + 1 < v.size())
			small_v.push_back(v[i + 1]);
		big_v.push_back(small_v);
	}
}


void PmergeMe::make_v_from_main_and_pend_chain(vec_vec_int main_chain, vec_int rest, vec_int &v)
{
	v.clear();
	for (size_t i = 0; i < main_chain.size(); i++)
	{
		for (size_t j = 0; j < main_chain[i].size(); j++)
			v.push_back(main_chain[i][j]);
	}
	for (size_t i = 0; i < rest.size(); i++)
		v.push_back(rest[i]);
}


void PmergeMe::merge_insertion_sort_list()
{
	list_list_int big_v;

	size_t size = numbers_list.size();

	unsigned long BeforeMicro = get_microseconds();
	merge_part_list(numbers_list, 0, big_v);
	unsigned long AfterMicro = get_microseconds();

	std::cout << "Time to process a range of " << size << " elements with std::list : " << (AfterMicro - BeforeMicro) << " us" << std::endl;
}

void PmergeMe::merge_part_list(list_int &v, int vec_size, list_list_int &big_v)
{
	int size_of_v = 2 * vec_size;
	if (size_of_v == 0)
		size_of_v = 1;

	make_pairs_list(v, size_of_v, big_v);
	if(list_debug_mode)
		print_list(big_v, vec_size);
	reorder_list(vec_size, big_v, v);

	if(big_v.size() > 3)
		merge_part_list(v, size_of_v, big_v);
	list_insertion_part(big_v, list_rest, vec_size, v);
}


void PmergeMe::make_pairs_list(list_int v, int size_of_v, list_list_int &big_v)
{
	if (size_of_v == 0)
		size_of_v = 1;
	big_v.clear();
	list_rest.clear();
	list_int small_v;
	for(size_t i = 0; i < v.size(); i += size_of_v)
	{
		if(i + size_of_v > v.size())
		{
			for(size_t j = i; j < v.size(); j++)
				list_rest.push_back(get_value(v.begin(), j));
			break;
		}
		small_v.clear();
		for(size_t j = i; j < i + size_of_v && j < v.size(); j++)
			small_v.push_back(get_value(v.begin(), j));
		big_v.push_back(small_v);
	}
}

void PmergeMe::list_insertion_part(list_list_int &big_v, list_int rest, int list_size, list_int &v)
{
	if(list_debug_mode)
		print_list(big_v, list_size);
	list_list_int main_chain;
	list_pair pend_chain;

	make_main_chain_and_pend_chain_list(main_chain, pend_chain, big_v);

	if(list_debug_mode)
		print_main_and_pend_chain_list(main_chain, pend_chain, rest, list_size);

    long long jacobsthal_diff[] = {
        2u, 2u, 6u, 10u, 22u, 42u, 86u, 170u, 342u, 682u, 1366u,
        2730u, 5462u, 10922u, 21846u, 43690u, 87382u, 174762u, 349526u, 699050u,
        1398102u, 2796202u, 5592406u, 11184810u, 22369622u, 44739242u, 89478486u,
        178956970u, 357913942u, 715827882u, 1431655766u, 2863311530u, 5726623062u,
        11453246122u, 22906492246u, 45812984490u, 91625968982u, 183251937962u,
        366503875926u, 733007751850u, 1466015503702u, 2932031007402u, 5864062014806u,
        11728124029610u, 23456248059222u, 46912496118442u, 93824992236886u, 187649984473770u,
        375299968947542u, 750599937895082u, 1501199875790165u, 3002399751580331u,
        6004799503160661u, 12009599006321322u, 24019198012642644u, 48038396025285288u,
        96076792050570576u, 192153584101141152u, 384307168202282304u, 768614336404564608u,
        1537228672809129216u, 3074457345618258432u, 6148914691236516864u
    };
	(void)jacobsthal_diff;
	size_t i = 0;
	while(pend_chain.size())
	{
		list_pair_iter begin = pend_chain.begin();
		list_pair_iter end = pend_chain.begin();
		int j = -1;
		while(++j < jacobsthal_diff[i] -1)
			end++;
		i++;
		if(distance_iter(pend_chain.begin(), end) >= pend_chain.size())
		{
			end = pend_chain.end();
			end--;
		}
		while(true)
		{
			list_list_iter it2 = std::lower_bound(main_chain.begin(), main_chain.end(), end->first, comp_func_list);

			main_chain.insert(it2, end->first);
			list_pair_iter to_delete = end;

			if (end == begin)
			{
				pend_chain.erase(to_delete);
				break;
			}
			--end;
			pend_chain.erase(to_delete);
		}
	}

	make_list_from_main_and_pend_chain(main_chain, rest, v);
	
	make_pairs_list(v, list_size, big_v);
}


void PmergeMe::make_list_from_main_and_pend_chain(list_list_int main_chain, list_int rest, list_int &v)
{
	v.clear();
	for(list_list_iter it = main_chain.begin(); it != main_chain.end(); it++)
		for (list_iter it2 = it->begin(); it2 != it->end(); it2++)
			v.push_back(*it2);
	for (list_iter it = rest.begin(); it != rest.end(); it++)
		v.push_back(*it);
}

bool PmergeMe::is_sorted_list(list_int v)
{
	for(list_iter it = v.begin(); it != v.end(); it++)
	{
		if(advance_iter(it, 1) == v.end())
			break;
		if(*it > *advance_iter(it, 1))
			return false;
	}
	return true;
}

bool PmergeMe::is_sorted_vect(vec_int v)
{
	for(vec_iter it = v.begin(); it != v.end(); it++)
	{
		if(it + 1 == v.end())
			break;
		if(*it > *(it + 1))
			return false;
	}
	return true;
}