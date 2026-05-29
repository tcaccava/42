
#include "Bureaucrat.hpp"

int main ()
{
    Bureaucrat *Billy = 0;
    Bureaucrat *Mario = 0;

    try
    {
        Billy = new Bureaucrat("Billy Ballo", 1);
    }
    catch (std::exception& e)
    {
        return (0);
    }
    std::cout << *Billy;
    try
    {
        Mario = new Bureaucrat("Mario Mariottide", 150);
    }
    catch (std::exception& e)
    {        
        delete Billy;
        return (0);
    }
    std::cout << *Mario;

    //first block
    try {
        Billy->decrementGrade();
    }
    catch (std::exception& e) {
        delete Mario;
        delete Billy;
        return 0;
    }
    try { 
        Mario->incrementGrade();
    }
    catch (std::exception& e) {
        delete Mario;
        delete Billy;
        return 0;
    }

    //second block
    try {
        Billy->incrementGrade();
    }
    catch (std::exception& e) {
        delete Mario;
        delete Billy;
        return 0;
    }
    try { 
        Mario->decrementGrade();
    }
    catch (std::exception& e) {
        delete Mario;
        delete Billy;
        return 0;
    }

    //third block
    try {
        Billy->incrementGrade();
    }
    catch (std::exception& e) {
        delete Mario;
        delete Billy;
        return 0;
    }
    try { 
        Mario->decrementGrade();
    }
    catch (std::exception& e) {
        delete Mario;
        delete Billy;
        return 0;
    }
    delete Mario;
    delete Billy;
}