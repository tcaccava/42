#include "Span.hpp"

Span::Span(unsigned int N) {
    max_size = N;
}

Span::~Span() {} 

Span::Span(const Span& other) {
    max_size = other.max_size;
    for(std::vector<int>::const_iterator it = other.container.begin();it != other.container.end();it++)
        this->container.push_back(*it);
}

Span &Span::operator=(const Span& other){
    if(this != & other){
        max_size = other.max_size;
        this->container.clear();
        for(std::vector<int>::const_iterator it = other.container.begin(); it != other.container.end(); it++)
            this->container.push_back(*it);
    }
    return *this;
}

// alternative with assign method
// Sign &Sign::operator=(const Sign& other){
//     if(this != & other){
//         max_size = other.max_size;
//         this->container.clear();
//         this->container.assign(other.container.begin(), other.container.end());
//     }
//     return *this
// }

const char* Span::Exception::what() const throw() {
    return ("Array is full");
}
   
const char * Span::NoSpan::what() const throw() {
    return ("No span found");
}

void Span::addNumber(int add){
    if (container.size() < max_size)
        container.push_back(add);
    else 
        throw Exception();
}

int Span::longestSpan() {
    if (container.empty() || container.size() == 1){
        throw NoSpan();
    }
    std::sort(container.begin(), container.end());
    return (container.back() - container.front());
}

int Span::shortestSpan() {
    if (container.empty() || container.size() == 1){
        throw NoSpan();
    }
    std::sort(container.begin(), container.end());
    int min = container.back() - container.front();
    for(std::vector<int>::iterator it = container.begin() + 1; it != container.end(); ++it){
    int temp = *it - *(it - 1);
    if (temp < min)
        min = temp;
    }
        return min;
    }

void Span::addNumbers(std::vector<int>::iterator first, std::vector<int>::iterator last){
    if(container.size() + std::distance(first, last) > max_size)
        throw Exception();
    container.insert(container.end(),first, last);
}