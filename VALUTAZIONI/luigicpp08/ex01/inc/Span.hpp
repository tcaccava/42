#ifndef SPAN_HPP
#define SPAN_HPP

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

class Span
{
    public:
    std::vector<int> integer_vector;
    unsigned int max_size;

    Span(unsigned int N);

    void addNumber(int num);
    unsigned int shortestSpan() const;
    unsigned int longestSpan() const;

    template<typename T>
    void insert_nums(T it_begin, T it_end)
    {
        size_t dist = std::distance(it_begin, it_end);

        if (dist + integer_vector.size()> max_size)
            throw std::length_error("no space");
        integer_vector.insert(integer_vector.end(), it_begin, it_end);
    }
};
    
#endif