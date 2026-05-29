#include "Intern.hpp"
#include "ShrubberyCreationForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"


AForm *Intern::shrubbery_function(std::string target)
{
  return new ShrubberyCreationForm(target);
}

AForm *Intern::robotomy_function(std::string target)
{
  return new RobotomyRequestForm(target);
}

AForm *Intern::presidential_function(std::string target)
{
  return new PresidentialPardonForm(target);
}

 AForm *Intern::makeForm(std::string name, std::string target)
 {
    
    std::string names[3] = {
      "Shrubbery Creation",
      "Presidential Pardon",
      "Robotomy Request"
    };

    AForm *(*functionArray[3])(std::string target) = {
      &Intern::shrubbery_function,
      &Intern::presidential_function,
      &Intern::robotomy_function
    };

    int i = 0;
    while (i < 3)
    {
      if (name == names[i])
      {  
        std::cout << "name: " << name << "; target: " << target << std::endl;
        return (functionArray[i](target));
      }
      i++;
    }
    std::cout << "The form doesn't exist!!\n";
    return NULL;
 }