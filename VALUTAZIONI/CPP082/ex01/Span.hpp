#ifndef SPAN_HPP
#define SPAN_HPP

#include <vector>
#include <stdexcept>
#include <algorithm>

class Span {
private:
    unsigned int _maxSize;
    std::vector<int> _numbers;

public:
    Span(unsigned int N);
    void addNumber(int number);

    template <typename Iterator>
    void addRange(Iterator begin, Iterator end) {
        while (begin != end) {
            addNumber(*begin);
            ++begin;
        }
    }

    int shortestSpan() const;
    int longestSpan() const;

    class FullException : public std::exception {
    public:
        virtual const char* what() const throw();
    };

    class NoSpanException : public std::exception {
    public:
        virtual const char* what() const throw();
    };
};

#endif