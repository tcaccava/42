#include "Bureaucrat.hpp"
#include <iostream>
#include <exception>
#include "Form.hpp"
int main()
{
    try
    {
        Bureaucrat a("Astolfo", 5);
        Form x("Modulo", 4,3);
        std::cout << a << std::endl;
        std::cout << x << std::endl;
        a.signForm(x);
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
   
}