#include "easyfind.hpp"
#include <iostream>

#include <vector>

int main()
{
    std::vector<int> integer_vector;
    integer_vector.push_back(1);
    integer_vector.push_back(2);
    integer_vector.push_back(3);

    int value = 4;

    try {
        std::vector<int>::iterator it = easyfind(integer_vector, value);
        std::cout << *it << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}