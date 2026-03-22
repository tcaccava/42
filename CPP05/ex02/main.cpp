#include "AForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"

int main() {
    try{
    std::cout << "TEST SHRUBBERY" << std::endl;
    Bureaucrat lowgrade("Lowgrade", 150);
    ShrubberyCreationForm cespuglio("garden");

    std::cout << cespuglio << std::endl;
    std::cout << lowgrade << std::endl;

    lowgrade.signForm(cespuglio);
    lowgrade.executeForm(cespuglio);

    Bureaucrat goodgrade("GoodGrade", 130);
    ShrubberyCreationForm cespuglio2("home");

    goodgrade.signForm(cespuglio2);
    goodgrade.executeForm(cespuglio2);

    std::cout << "File 'home_shrubbery' should have been created" << std::endl;
}catch(const std::exception &e) {
    std::cout << "Exception caught: " << e.what() << std::endl;
}
try{
    std::cout << "TEST ROBOTOMY" << std::endl;
    Bureaucrat lowgrade("lowrobot", 80);
    RobotomyRequestForm robot("Bender");

    std::cout << robot << std::endl;
    std::cout << lowgrade << std::endl;

    lowgrade.signForm(robot);
    lowgrade.executeForm(robot);

    Bureaucrat goodgrade("HighRobot", 40);
    RobotomyRequestForm robot2("Neeno");

    goodgrade.signForm(robot2);
    goodgrade.executeForm(robot2);   
}
catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
try {
        std::cout << "TEST PARDON" << std::endl;
        Bureaucrat lowGrade("LowPresident", 30);
        PresidentialPardonForm pardon("Arthur Dent");
        
        std::cout << pardon << std::endl;
        std::cout << lowGrade << std::endl;
        
        lowGrade.signForm(pardon);
        lowGrade.executeForm(pardon);
        
        Bureaucrat president("President", 1);
        PresidentialPardonForm pardon2("Ford Prefect");
        
        std::cout << president << std::endl;
        president.signForm(pardon2);
        president.executeForm(pardon2);
        
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    try {
        std::cout <<"TEST FORM SIGN" << std::endl;
        Bureaucrat boss("Boss", 1);
        ShrubberyCreationForm cespuglio3("office");
        
        boss.executeForm(cespuglio3);
        
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
}

