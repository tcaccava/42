#include "Iter.hpp"
#include <iostream>

void reader(int i)
{
    std::cout << i << "\n";
}

void readstr(std::string i)
{
    std::cout << i << "\n";
}

int main()
{
    int array[4] = {1,2,3,4};
    iter(array,4,reader);

    std::string ar[4] = {"a", "b", "c", "d"};
    iter(ar, 4, readstr);
}