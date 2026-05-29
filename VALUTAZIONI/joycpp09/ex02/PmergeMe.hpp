#ifndef PmergeMe_HPP
#define PmergeMe_HPP

#include <iostream>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <cstddef>
#include <utility>
#include <vector>
#include <list>
#include <sys/time.h>

// VECTOR

typedef std::vector<int> vec_int;
typedef vec_int::iterator vec_iter;

typedef std::vector<std::vector<int> > vec_vec_int;
typedef vec_vec_int::iterator vec_vec_iter;


typedef std::vector<std::pair<vec_int, vec_vec_iter> > vec_pair;
typedef vec_pair::iterator vec_pair_iter;

// LIST

typedef std::list<int> list_int;
typedef list_int::iterator list_iter;

typedef std::list<std::list<int> > list_list_int;
typedef list_list_int::iterator list_list_iter;

typedef std::list<std::pair<list_int, list_list_iter> > list_pair;
typedef list_pair::iterator list_pair_iter;


class PmergeMe
{
	private:
		vec_int numbers_vec;
		list_int numbers_list;

		vec_int vec_rest;
		list_int list_rest;
		bool vec_debug_mode;
		bool list_debug_mode;
		PmergeMe();
	public:
		PmergeMe(char **av);
		~PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		void merge_insertion_sort_vector();
		void merge_part_vector(vec_int &v, int vec_size, vec_vec_int &big_v);
		void print_vec(vec_vec_int big_v, int vec_size);
		void reorder_v(int vec_size, vec_vec_int &big_v,vec_int &v);
		void make_new_v(vec_vec_int &big_v,vec_int &new_v);
		void make_pairs_vec(vec_int v, int size_of_v, vec_vec_int &big_v);
		void vect_insertion_part(vec_vec_int &big_v,vec_int rest, int vec_size,vec_int &new_v);
		void print_main_and_pend_chain_vect(vec_vec_int main_chain, vec_pair pend_chain,vec_int rest, int vec_size);
		void split_big_v(vec_vec_int &big_v,vec_int v, int vec_size);
		void make_v_from_main_and_pend_chain(vec_vec_int main_chain,vec_int rest,vec_int &v);
		void update_pend_chain_vec(vec_pair &pend_chain, vec_vec_iter it);
		void make_main_chain_and_pend_chain_vec(vec_vec_int &main_chain, vec_pair &pend_chain, vec_vec_int big_v);
		void set_vect_debug_mode(bool debug_mode);
		void main_print_vec(vec_int v);

		void merge_insertion_sort_list();
		void merge_part_list(list_int &v, int vec_size, list_list_int &big_v);
		void make_pairs_list(list_int v, int size_of_v, list_list_int &big_v);
		void print_list(list_list_int big_v, int list_size);
		void reorder_list(int list_size, list_list_int &big_v, list_int &v);
		void make_new_list(list_list_int &big_v,list_int &new_v);
		void list_insertion_part(list_list_int &big_v,list_int rest, int list_size,list_int &new_v);
		void make_main_chain_and_pend_chain_list(list_list_int &main_chain, list_pair &pend_chain, list_list_int big_v);
		void print_main_and_pend_chain_list(list_list_int main_chain, list_pair pend_chain,list_int rest, int list_size);
		void make_list_from_main_and_pend_chain(list_list_int main_chain,list_int rest,list_int &v);
		void set_list_debug_mode(bool debug_mode);
		void main_print_list(list_int v);

		bool is_sorted_vect(vec_int v);
		bool is_sorted_list(list_int v);

		template <typename T>
		T advance_iter(T it, int n)
		{
			for(int i = 0; i < n; i++)
				it++;
			return it;
		}
		
		template<typename T>
		size_t distance_iter(T it1, T it2)
		{
			size_t i = 0;
			while(it1 != it2)
			{
				it1++;
				i++;
			}
			return i;
		}
		
		// template<typename T>
		int get_value(list_iter it, int n)
		{
			for(int i = 0; i < n; i++)
				it++;
			return *it;
		}

};		

bool comp_func_vec(vec_int v1, vec_int v2);
bool comp_func_list(list_int v1, list_int v2);

#endif