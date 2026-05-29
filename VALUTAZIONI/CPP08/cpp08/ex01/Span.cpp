/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Span.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcalabre <pcalabre@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:33:20 by pcalabre          #+#    #+#             */
/*   Updated: 2025/03/04 18:33:21 by pcalabre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Span.hpp"
#include <algorithm>
#include <vector>

Span::Span() {}

Span::Span(const Span &s) { *this = s; }

Span::Span(int n) : _len(n) {}

Span::~Span() {}

Span &Span::operator=(const Span &s) {
    if (this == &s)
        return *this;

    _data = s._data;
    return *this;
}

void Span::addNumber(int n) {
    if (_data.size() == static_cast<unsigned long>(_len))
        throw OutOfRange();
    _data.push_back(n);
}

unsigned int Span::longestSpan() {
    if (_len < 2 || _data.size() < 2)
        throw SizeTooSmall();
    return *std::max_element(_data.begin(), _data.end()) -
           *std::min_element(_data.begin(), _data.end());
}

unsigned int Span::shortestSpan() {
    if (_len < 2 || _data.size() < 2)
        throw SizeTooSmall();
    std::vector<int> copy(_data);
    std::sort(copy.begin(), copy.end());
    int span = copy[1] - copy[0];
    for (std::vector<int>::iterator it = copy.begin(); it != copy.end() - 1;
         it++) {
        if (*(it + 1) - *it < span)
            span = *(it + 1) - *it;
    }
    return span;
}
