#include "Span.hpp"

/*
int main(void) 
{
    Span contenitore(3);
    
    std::cout << "this is maximum size: "<< contenitore.max_size << "\n";
    std::cout << "this is the size of vector: " << contenitore.integer_vector.size() << "\n";
    contenitore.addNumber(1);
    std::cout << "this is the size of vector: " << contenitore.integer_vector.size() << "\n";
    contenitore.addNumber(1);
    std::cout << "this is the size of vector: " << contenitore.integer_vector.size() << "\n";
    contenitore.addNumber(1);
    std::cout << "this is the size of vector: " << contenitore.integer_vector.size() << "\n";

    


    return 0;
}

*/

//test 2 maiin of subject

int main()
{
    Span sp = Span(5);
    sp.addNumber(6);
    sp.addNumber(3);
    sp.addNumber(17);
    sp.addNumber(9);
    sp.addNumber(11);
    std::cout << sp.shortestSpan() << std::endl;
    std::cout << sp.longestSpan() << std::endl;
    return 0;
}