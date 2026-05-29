#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "AForm.hpp"
#include "Intern.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

int main()
{
    srand(time(NULL));

    std::cout << "=== TEST EX03 - INTERN ===" << std::endl;

    Intern someRandomIntern;
    AForm *form;

    std::cout << "\n--- Test shrubbery creation ---" << std::endl;
    form = someRandomIntern.makeForm("shrubbery creation", "Bender");
    if (form) {
        std::cout << "Form created: " << *form << std::endl;
        delete form;
        form = NULL;
    }

    std::cout << "\n--- Test robotomy request ---" << std::endl;
    form = someRandomIntern.makeForm("robotomy request", "Alice");
    if (form) {
        std::cout << "Form created: " << *form << std::endl;
        delete form;
        form = NULL;
    }

    std::cout << "\n--- Test presidential pardon ---" << std::endl;
    form = someRandomIntern.makeForm("presidential pardon", "Bob");
    if (form) {
        std::cout << "Form created: " << *form << std::endl;
        delete form;
        form = NULL;
    }

    std::cout << "\n--- Test forme inconnue ---" << std::endl;
    form = someRandomIntern.makeForm("unknown form", "target");
    if (!form) {
        std::cout << "Form creation failed as expected" << std::endl;
    }

    return 0;
}
