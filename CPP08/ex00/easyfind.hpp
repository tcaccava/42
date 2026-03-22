#ifndef EASYFIND_HPP
#define EASYFIND_HPP

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <deque>
#include <set>

class Exception : public std::exception {
    public:
        virtual const char* what() const throw(){
            return "No occurrence found in the container";
        }
};

template<typename T>
typename T::iterator easyfind(T& container, int value) {
   typename T::iterator it = std::find(container.begin(), container.end(),value);
    if(it == container.end())
        throw Exception();
    return it;
}

#endif

// ITERATOR FOR ALTERNATIVE IS WRONG

// template<typename T>
// typename T::iterator easyfind(T& container, int value){
//     for(typename T::iterator it = container.begin(), it != container.end(); it++)
//         if(*it == n)
//             return it;
//     throw Exception();
// }