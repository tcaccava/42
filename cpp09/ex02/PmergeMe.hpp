#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <deque>
#include <list>
#include <climits>
#include <utility>
#include <algorithm>
#include <set>
#include <chrono>


class PmergeMe {
    public:
    PmergeMe();
    ~PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe &operator=(const PmergeMe& other);

    bool is_valid(const char* str);
    void add2_arrays(const char *str);
    void create_dcouples(std::deque<int> array);
    void create_lcouples(std::list<int> array);
    void create_dmain(std::deque<std::pair<int,int> > array);
    void create_lmain(std::list<std::pair<int,int> > array);
    void create_dpending(std::deque<std::pair<int,int> > array);
    void create_lpending(std::list<std::pair<int,int> > array);
    void gen_jacob(int n, std::deque<int>& jacobsthal);
    void dinsert(std::deque<int> &array);
    void linsert(std::list<int> &array);

    std::deque<int> deque;
    std::list<int> list;
    std::deque<std::pair<int,int> > deque_pairs;
    std::list<std::pair<int,int> > list_pairs;
    std::deque<int> deque_main_chain;
    std::deque<int> deque_pending;
    std::list<int> list_main_chain;
    std::list<int> list_pending;
    std::deque<int> jacobsthal;

};

#endif
