
#include "Intern.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

#include <iostream>

Intern::Intern()
{
}
Intern::Intern(const Intern&)
{
}

Intern::~Intern()
{
}

Intern& Intern::operator=(const Intern&)
{
	return *this;
}

AForm* Intern::makeForm(const std::string& formName, const std::string& target) const
{
    AForm* form = NULL;
	if (formName != "shrubbery creation" && formName != "robotomy request" && formName != "presidential pardon")
	{
		std::cerr << "Intern error: \"" << formName << "\" is not a valid form name" << std::endl;
		return (NULL);
	}
    switch ( formName.c_str()[0] )
    {
      case 's':
        form = new ShrubberyCreationForm(target);
        break;
      case 'r':
        form = new RobotomyRequestForm(target);
        break;
      case 'p':
        form = new PresidentialPardonForm(target);
        break;
      default:
        std::cerr << "Intern error: \"" << formName
                  << "\" is not a valid form name" << std::endl;
        return NULL;
    }

    std::cout << "Intern creates " << form->getName() << std::endl;
    return form;
}
