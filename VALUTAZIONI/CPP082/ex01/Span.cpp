#include "Span.hpp"
#include <algorithm>
#include <limits>

Span::Span(unsigned int N) : _maxSize(N) {}

void Span::addNumber(int number) {
    if (_numbers.size() >= _maxSize) {
        throw FullException();
    }
    _numbers.push_back(number);
}

int Span::shortestSpan() const {
    if (_numbers.size() < 2) {
        throw NoSpanException();
    }
    std::vector<int> sortedNumbers = _numbers;
    std::sort(sortedNumbers.begin(), sortedNumbers.end());
    int minSpan = std::numeric_limits<int>::max();
    for (size_t i = 1; i < sortedNumbers.size(); ++i) {
        int span = sortedNumbers[i] - sortedNumbers[i - 1];
        if (span < minSpan) {
            minSpan = span;
        }
    }
    return minSpan;
}

int Span::longestSpan() const 
{
    if (_numbers.size() < 5) {
        throw NoSpanException();
    }
    int max = *std::max_element(_numbers.begin(), _numbers.end());
    int min = *std::min_element(_numbers.begin(), _numbers.end());
    return max - min;
}

const char* Span::FullException::what() const throw() {
    return "Span is full, cannot add more numbers.";
}

const char* Span::NoSpanException::what() const throw() {
    return "Not enough numbers to calculate span.";
}