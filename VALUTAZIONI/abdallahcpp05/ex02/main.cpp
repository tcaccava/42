#include "Bureaucrat.hpp"
#include "RobotomyRequestForm.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(NULL));
    
    std::cout << "=== TEST EX02 - CONCRETE FORMS ===" << std::endl;
    
    std::cout << "\n--- Test ShrubberyCreationForm ---" << std::endl;
    try
    {
        Bureaucrat gardener("Gardener", 130);
        ShrubberyCreationForm shrub("home");
        
        std::cout << shrub << std::endl;
        gardener.signForm(shrub);
        gardener.executeForm(shrub);
        std::cout << "Check if 'home_shrubbery' file was created!" << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Erreur: " << e.what() << std::endl;
    }
    
    std::cout << "\n--- Test RobotomyRequestForm ---" << std::endl;
    try
    {
        Bureaucrat doctor("Doctor", 40);
        RobotomyRequestForm robot("Bender");
        
        std::cout << robot << std::endl;
        doctor.signForm(robot);
        doctor.executeForm(robot);
        doctor.executeForm(robot);
    }
    catch (std::exception &e)
    {
        std::cout << "Erreur: " << e.what() << std::endl;
    }
    
    std::cout << "\n--- Test PresidentialPardonForm ---" << std::endl;
    try
    {
        Bureaucrat president("President", 1);
        PresidentialPardonForm pardon("Arthur Dent");
        
        std::cout << pardon << std::endl;
        president.signForm(pardon);
        president.executeForm(pardon);
    }
    catch (std::exception &e)
    {
        std::cout << "Erreur: " << e.what() << std::endl;
    }
    
    std::cout << "\n--- Test échecs ---" << std::endl;
    try
    {
        Bureaucrat intern("Intern", 150);
        PresidentialPardonForm impossible("Someone");
        
        intern.signForm(impossible);      // Échec signature
        intern.executeForm(impossible);   // Échec exécution
    }
    catch (std::exception &e)
    {
        std::cout << "Erreur: " << e.what() << std::endl;
    }
    
    return 0;
}
