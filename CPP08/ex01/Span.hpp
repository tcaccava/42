#ifndef SPAN_HPP
#define SPAN_HPP

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
class Span
{
public:
    Span(unsigned int N);
    ~Span();
    Span(const Span &other);
    Span &operator=(const Span &other);

    void addNumber(int add);
    int shortestSpan();
    int longestSpan();
    void addNumbers(std::vector<int>::iterator first, std::vector<int>::iterator last);
    class Exception : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class NoSpan : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

private:
    std::vector<int> container;
    unsigned int max_size;
};

#endif