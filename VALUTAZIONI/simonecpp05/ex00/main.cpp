#include "Bureaucrat.hpp"
#include <iostream>
#include <typeinfo>

int main()
{
    try
    {
        Bureaucrat fire("Marius", 15);

        std::cout << fire;
        fire.incrementGrade();
        std::cout << fire;
        fire.incrementGrade();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << "Type: " << typeid(e).name() << std::endl;
    }
}
