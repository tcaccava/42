#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "Intern.hpp"

int main()
{
    Intern someRandomIntern;
    AForm* form;

    std::cout << "=== TEST 1: Robotomy Request ===" << std::endl;
    form = someRandomIntern.makeForm("robotomy request", "Bender");
    if (form)
    {
        std::cout << *form << std::endl;
        delete form;
    }

    std::cout << "\n=== TEST 2: Shrubbery Creation ===" << std::endl;
    form = someRandomIntern.makeForm("shrubbery creation", "Garden");
    if (form)
    {
        std::cout << *form << std::endl;
        delete form;
    }

    std::cout << "\n=== TEST 3: Presidential Pardon ===" << std::endl;
    form = someRandomIntern.makeForm("presidential pardon", "Arthur");
    if (form)
    {
        std::cout << *form << std::endl;
        delete form;
    }

    std::cout << "\n=== TEST 4: Form inesistente ===" << std::endl;
    form = someRandomIntern.makeForm("unknown form", "Nobody");
    if (form)
        delete form;

    std::cout << "\n=== TEST 5: Workflow completo ===" << std::endl;
    {
        Intern intern;
        Bureaucrat boss("Boss", 1);
        AForm* rrf = intern.makeForm("robotomy request", "Bender");
        if (rrf)
        {
            boss.signForm(*rrf);
            boss.executeForm(*rrf);
            delete rrf;
        }
    }

    return 0;
}
