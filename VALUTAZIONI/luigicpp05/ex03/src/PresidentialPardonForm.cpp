#include "PresidentialPardonForm.hpp"
#include <unistd.h>   //for sleep

PresidentialPardonForm::PresidentialPardonForm(std::string target): AForm(target, 25, 5){}

void PresidentialPardonForm::execute(Bureaucrat const &executor)
{
    std::cout << "Bureaucrat " << executor.getName() << ": execution in progress ...\n" << std::flush;;
    sleep(1);
    if (!this->getSigned())
       throw  (GradeTooLowException());
    if (this->getGradeToExec() < executor.getGrade())
        throw (GradeTooLowException());
    //specific behaviour ...overloading
    std::cout << getName() << " has pardoned by Zaphod Beeblebrox." << std::endl;

    std::cout << "The bureucrat named " << executor.getName() << " has executed the form\n";
}
