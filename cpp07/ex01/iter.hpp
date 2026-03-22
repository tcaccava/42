#ifndef ITER_HPP
#define ITER_HPP

#include <string>
#include <iostream>
template<typename P>
void print(const P &print){
    std::cout << print << std::endl;
}
template <typename T>
void iter(T* array, size_t length, void(*f)(const T &)){
    for(int i = 0; i < length ; ++i)
        f(array[i]);
}

#endif