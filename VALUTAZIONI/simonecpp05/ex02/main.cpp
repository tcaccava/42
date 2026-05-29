#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main()
{
    std::cout << "=== TEST ShrubberyCreationForm ===" << std::endl;
    {
        Bureaucrat bob("Bob", 100);
        ShrubberyCreationForm shrub("home");
        std::cout << shrub << std::endl;
        bob.signForm(shrub);
        bob.executeForm(shrub);
    }

    std::cout << "\n=== TEST RobotomyRequestForm ===" << std::endl;
    {
        Bureaucrat alice("Alice", 40);
        RobotomyRequestForm robot("Bender");
        std::cout << robot << std::endl;
        alice.signForm(robot);
        alice.executeForm(robot);
    }

    std::cout << "\n=== TEST PresidentialPardonForm ===" << std::endl;
    {
        Bureaucrat president("President", 1);
        PresidentialPardonForm pardon("Arthur Dent");
        std::cout << pardon << std::endl;
        president.signForm(pardon);
        president.executeForm(pardon);
    }

    std::cout << "\n=== TEST: Esecuzione senza firma ===" << std::endl;
    {
        Bureaucrat bob("Bob", 1);
        PresidentialPardonForm pardon("Ford");
        bob.executeForm(pardon);  // deve fallire: non firmato
    }

    std::cout << "\n=== TEST: Grade troppo basso per eseguire ===" << std::endl;
    {
        Bureaucrat intern("Intern", 100);
        PresidentialPardonForm pardon("Zaphod");
        Bureaucrat boss("Boss", 1);
        boss.signForm(pardon);
        intern.executeForm(pardon);  // deve fallire: grade troppo basso
    }

    return 0;
}
