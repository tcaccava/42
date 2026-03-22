#include "PmergeMe.hpp"

int main(int c, char **v)
{
    PmergeMe alg;
    if(c < 3){
        std::cerr << "Required two numbers al least" << std::endl;
        return 1;
    }
    for (int i = 1; i <= c - 1; i++)
    {
        if (!alg.is_valid(v[i]))
        {
            std::cerr << "Error: only positive numbers are allowed" << std::endl;
            return 1;
        }
    }
    for (int i = 1; i < c; i++)
        alg.add2_arrays(v[i]);
    std::chrono::high_resolution_clock::time_point start_deque = std::chrono::high_resolution_clock::now();
    alg.create_dcouples(alg.deque);
    alg.create_dmain(alg.deque_pairs);
    alg.create_dpending(alg.deque_pairs);
    std::sort(alg.deque_main_chain.begin(), alg.deque_main_chain.end());
    alg.dinsert(alg.deque_pending);
    std::chrono::high_resolution_clock::time_point end_deque = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds time_deque = std::chrono::duration_cast<std::chrono::microseconds>(end_deque - start_deque);

    std::chrono::high_resolution_clock::time_point start_list = std::chrono::high_resolution_clock::now();
    alg.create_lcouples(alg.list);
    alg.create_lmain(alg.list_pairs);
    alg.create_lpending(alg.list_pairs);
    alg.list_main_chain.sort();
    alg.linsert(alg.list_pending);
    std::chrono::high_resolution_clock::time_point end_list = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds time_list = std::chrono::duration_cast<std::chrono::microseconds>(end_list - start_list);

    std::cout << "Before:  ";
    for (int i = 1; i < c; i++)
        std::cout << v[i] << " ";
    std::cout << std::endl;
    std::cout << "After:   ";
    for (int i = 0; i < alg.deque_main_chain.size(); i++)
        std::cout << alg.deque_main_chain[i] << " ";
    std::cout << std::endl;
    std::cout << "Time to process a range of " <<  alg.deque.size() << " elements with std::deque : "<< time_deque.count() << " us" << std::endl;
    std::cout << "Time to process a range of " <<  alg.list.size() << " elements with std::list  : "<< time_list.count() << " us" << std::endl;

    
    
    
    // for (std::list<int>::iterator it = alg.list_main_chain.begin(); it != alg.list_main_chain.end(); it++)
    //     std::cout << *it << std::endl;
    // for (int i = 0; i < alg.deque_pending.size(); i++)
    //     std::cout << alg.deque_pending[i] << std::endl;
    // for (std::list<int>::iterator it = alg.list_pending.begin(); it != alg.list_pending.end(); it++)
    //     std::cout << *it << std::endl;
}
