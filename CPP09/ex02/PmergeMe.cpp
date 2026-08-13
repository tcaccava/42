#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other)
{
    this->deque = other.deque;
    this->list = other.list;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        deque.clear();
        list.clear();
        this->deque = other.deque;
        this->list = other.list;
    }
    return *this;
}

bool PmergeMe::is_valid(const char *str)
{
    char *endptr;
    errno = 0;
    long number = strtol(str, &endptr, 10);
    if (*endptr != '\0')
        return false;
    if ((number == LONG_MAX || number == LONG_MIN) && errno == ERANGE)
        return false;
    if (number < INT_MIN || number > INT_MAX)
        return false;
    if (number < 0)
        return false;
    return true;
}

void PmergeMe::add2_arrays(const char *str)
{
    long number = strtol(str, NULL, 10);
    deque.push_back(number);
    list.push_back(number);
}

void PmergeMe::create_dcouples(std::deque<int> array)
{
    for (int i = 0; i < array.size() - 1; i = i + 2)
    {
        int a = array[i];
        int b = array[i + 1];
        if (a < b)
            deque_pairs.push_back(std::make_pair(a, b));
        else
            deque_pairs.push_back(std::make_pair(b, a));
    }
    if (array.size() % 2 == 1)
        deque_pending.push_back(array.back());
}

void PmergeMe::create_lcouples(std::list<int> array)
{
    for (std::list<int>::iterator it = array.begin(); it != array.end();)
    {
        int a = *it;
        it++;
        if (it == array.end())
        {
            list_pending.push_back(a);
            break;
        }
        int b = *it;
        it++;
        if (a < b)
            list_pairs.push_back(std::make_pair(a, b));
        else
            list_pairs.push_back(std::make_pair(b, a));
    }
}

void PmergeMe::create_dmain(std::deque<std::pair<int, int>> array)
{
    for (std::deque<std::pair<int, int>>::iterator it = array.begin(); it != array.end(); it++)
    {
        deque_main_chain.push_back(it->second);
    }
}

void PmergeMe::create_lmain(std::list<std::pair<int, int>> array)
{
    for (std::list<std::pair<int, int>>::iterator it = array.begin(); it != array.end(); it++)
    {
        list_main_chain.push_back(it->second);
    }
}

void PmergeMe::create_dpending(std::deque<std::pair<int, int>> array)
{
    for (std::deque<std::pair<int, int>>::iterator it = array.begin(); it != array.end(); it++)
    {
        deque_pending.push_back(it->first);
    }
}

void PmergeMe::create_lpending(std::list<std::pair<int, int>> array)
{
    for (std::list<std::pair<int, int>>::iterator it = array.begin(); it != array.end(); it++)
    {
        list_pending.push_back(it->first);
    }
}

void PmergeMe::gen_jacob(int n, std::deque<int> &jacobsthal)
{
    jacobsthal.clear();
    if (n < 1)
        return;
    if (n == 1)
    {
        jacobsthal.push_back(0);
        return;
    }
    if (n == 2)
    {
        jacobsthal.push_back(0);
        jacobsthal.push_back(1);
        return;
    }
    if (n == 3)
    {
        jacobsthal.push_back(0);
        jacobsthal.push_back(1);
    }
    if (n > 3)
    {
        jacobsthal.push_back(0);
        jacobsthal.push_back(1);
        jacobsthal.push_back(3);
        while (1)
        {
            int j = jacobsthal[jacobsthal.size() - 1] + 2 * jacobsthal[jacobsthal.size() - 2];
            if (j < n)
                jacobsthal.push_back(j);
            else
                return;
        }
    }
}
void PmergeMe::dinsert(std::deque<int> &array)
{
    int size = array.size();
    gen_jacob(size, jacobsthal);
    std::set<int> indexes;
    for (int i = 0; i < jacobsthal.size(); i++)
    {
        int index = jacobsthal[i];
        if (index >= size)
            break;
        indexes.insert(index);
        std::deque<int>::iterator pos = std::lower_bound(deque_main_chain.begin(),deque_main_chain.end(),array[index]);
        deque_main_chain.insert(pos, array[index]);
        // bool inserted = false;
        // for (std::deque<int>::iterator it = deque_main_chain.begin(); it != deque_main_chain.end();)
        // {
        //     if (*it > array[index])
        //     {
        //         deque_main_chain.insert(it, array[index]);
        //         inserted = true;
        //         break;
        //     }
        //     else
        //         it++;
        // }
        // if (!inserted)
        //     deque_main_chain.push_back(array[index]);
    }
    for (int i = 0; i < array.size(); i++)
    {
        if (indexes.find(i) == indexes.end())
        {
            
            std::deque<int>::iterator pos = std::lower_bound(
                deque_main_chain.begin(), 
                deque_main_chain.end(), 
                array[i]
            );
            deque_main_chain.insert(pos, array[i]);
            // bool inserted = false;
            // for (std::deque<int>::iterator it = deque_main_chain.begin(); it != deque_main_chain.end();)
            // {
            //     if (*it > array[i])
            //     {
            //         deque_main_chain.insert(it, array[i]);
            //         inserted = true;
            //         break;
            //     }
            //     else
            //         it++;
            // }
            // if (!inserted)
            //     deque_main_chain.push_back(array[i]);
        }
    }
    array.clear();
}
void PmergeMe::linsert(std::list<int> &array)
{
    int size = array.size();
    gen_jacob(size, jacobsthal);
    std::deque<int> values;
    for (std::list<int>::iterator it = array.begin(); it != array.end(); it++)
        values.push_back(*it);
    std::set<int> indexes;
    for (int i = 0; i < jacobsthal.size(); i++)
    {
        int index = jacobsthal[i];
        if (index >= size)
            break;
        indexes.insert(index);
        std::list<int>::iterator pos = std::lower_bound(list_main_chain.begin(), list_main_chain.end(), values[index]);
        list_main_chain.insert(pos, values[index]);
    }
        // bool inserted = false;
        // for (std::list<int>::iterator it = list_main_chain.begin(); it != list_main_chain.end();)
        // {
        //     if (*it > values[index])
        //     {
        //         list_main_chain.insert(it, values[index]);
        //         inserted = true;
        //         break;
        //     }
        //     else
        //         it++;
        // }
        // if (!inserted)
        //     list_main_chain.push_back(values[index]);
    

    for (int i = 0; i < array.size(); i++)
    {
        if (indexes.find(i) == indexes.end())
        {
            std::list<int>::iterator pos = std::lower_bound(list_main_chain.begin(), list_main_chain.end(), values[i]);
            list_main_chain.insert(pos, values[i]);
            // bool inserted = false;
            // for (std::list<int>::iterator it = list_main_chain.begin(); it != list_main_chain.end();)
            // {
            //     if (*it > values[i])
            //     {
            //         list_main_chain.insert(it, values[i]);
            //         inserted = true;
            //         break;
            //     }
            //     else
            //         it++;
            // }
            // if (!inserted)
            //     list_main_chain.push_back(values[i]);
        }
    }
}