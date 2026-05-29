#include "Span.hpp"

Span::Span(unsigned int N): max_size(N)
{
    integer_vector.reserve(N);
}

void Span::addNumber(int num)
{
    if (this->integer_vector.size() >= this->max_size)
            throw std::length_error("you reached max size!");
    this->integer_vector.push_back(num);
}


unsigned int Span::shortestSpan() const
{
    if (integer_vector.size() < 2)
        throw std::length_error("not enough numbers");

    std::vector<int> tmp(integer_vector);

    std::sort(tmp.begin(), tmp.end());

    int min_span = std::numeric_limits<int>::max();

    for (size_t i = 1; i < tmp.size(); ++i)
    {
        int diff = tmp[i] - tmp[i - 1];

        if (diff < min_span)
            min_span = diff;
    }

    return min_span;
}


unsigned int  Span::longestSpan() const
{
    if (integer_vector.size() < 2)
        throw std::length_error("not enough numbers");

    std::vector<int> tmp(this->integer_vector);
    std::sort(tmp.begin(),tmp.end());

    int bigger_num = tmp[tmp.size() - 1];
    int smaller_num = tmp[0];



    int difference = bigger_num - smaller_num;



    return static_cast<unsigned int>(difference);
}