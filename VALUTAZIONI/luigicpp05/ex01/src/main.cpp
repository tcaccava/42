

#include "Bureaucrat.hpp"
#include "Form.hpp"

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
    
    Form *b;
    try {
        b = new Form("atto notarile", 10, 10);
    }
    catch (std::exception& e){
        std::cout << e.what() << std::endl;
        return 0;
    }
    try
    {
        Mario->signForm(*b);
    }
    catch(std::exception& e)
    {
            delete b;
            delete Mario;
            delete Billy;
        return 0;
    }
    try
    {
        Billy->signForm(*b);
    }
    catch(std::exception& e)
    {
        return 0;
    }

    delete b;
    delete Mario;
    delete Billy;
}
