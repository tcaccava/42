#include <iostream>
#include "Span.hpp"
#include <vector>

int main() {
    try {
        Span sp = Span(5);
        sp.addNumber(6);
        sp.addNumber(3);
        sp.addNumber(17);
        sp.addNumber(9);
        sp.addNumber(11);

        std::cout << "Shortest span: " << sp.shortestSpan() << std::endl;
        std::cout << "Longest span: " << sp.longestSpan() << std::endl;

        std::vector<int> moreNumbers;
        moreNumbers.push_back(100);
        moreNumbers.push_back(200);
        moreNumbers.push_back(300);
        moreNumbers.push_back(400);
        moreNumbers.push_back(500);
        moreNumbers.push_back(600);
        moreNumbers.push_back(700);

        sp.addRange(moreNumbers.begin(), moreNumbers.end());

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}