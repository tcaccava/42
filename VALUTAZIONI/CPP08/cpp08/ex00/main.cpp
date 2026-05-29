/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcalabre <pcalabre@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:32:54 by pcalabre          #+#    #+#             */
/*   Updated: 2025/03/04 18:40:05 by pcalabre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easyfind.hpp"

#include <deque>
#include <list>
#include <vector>

template <typename T> void test() {
    int arr[7] = {4, 6, 8, 10, 1, 2, 5};
    int target = 5;
    T data(arr, arr + 7);

    typename T::iterator it = easyfind(data, target);
    if (it != data.end()) {
        std::cout << "Found occurence: " << *it << std::endl;
    } else {
        std::cout << "Couldn't find occurence" << std::endl;
    }
}

int main() {
    test< std::list<int> >();
    test< std::vector<int> >();
    test< std::deque<int> >();
    return 0;
}

// int main() {
//     // Initialize a set
//     std::vector<int> v;
//     v.push_back(4);
//     v.push_back(6);
//     v.push_back(8);
//     v.push_back(10);
//     v.push_back(1);
//     v.push_back(2);
//     v.push_back(5);

//     int target = 1;

//     std::vector<int>::iterator it = std::find(v.begin(), v.end(), target);

//     // if target is found print it's index
//     if (it != v.end()) {
//         std::cout << "The first occurrence of " << target
//                   << " is at index: " << it - v.begin() << std::endl;
//     }
//     // else element not found
//     else {
//         std::cout << "Element not found." << std::endl;
//     }

//     return 0;
// }
