#include "Bureaucrat.hpp"
#include <iostream>
#include <exception>
int main()
{
    try
    {
        Bureaucrat a("Astolfo", 5);
        std::cout << a << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    try
    {
        Bureaucrat b("Geeno", 0);
        std::cout << b << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    try
    {
        Bureaucrat c("Geppino", 152);
        std::cout << c << std::endl;
    }
    catch(std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
