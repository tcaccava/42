#include "Intern.hpp"
#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"

Intern::Intern()
{
	//std::cout << "Default constructor called Intern" << std::endl;
}

Intern::Intern(Intern &copy)
{
	(void) copy;
	//std::cout << "Copy constructor called Intern" << std::endl;
}

//throw find a issue, he give it to the exception
// try/ catch try to find a solution for the issue
Intern::~Intern()
{
	//std::cout << "Destructor called Intern" << std::endl;
}

Intern &Intern::operator=(const Intern &other)
{
	(void)other; // Pas d'attributs à assigner
	return *this;
}

AForm *Intern::createShrubberyCreationForm(const std::string &target)
{
	return new ShrubberyCreationForm(target);
}

AForm *Intern::createRobotomyRequestForm(const std::string &target)
{
	return new RobotomyRequestForm(target);
}

AForm *Intern::createPresidentialPardonForm(const std::string &target)
{
	return new PresidentialPardonForm(target);
}


AForm *Intern::makeForm(const std::string formName, const std::string formTarget)
{
	std::string formTypes[3];
	formTypes[0] = "shrubbery creation";
	formTypes[1] = "robotomy request"; 
	formTypes[2] = "presidential pardon";

	AForm *(Intern::*formCreators[3])(const std::string &);
	formCreators[0] = &Intern::createShrubberyCreationForm;
	formCreators[1] = &Intern::createRobotomyRequestForm;
	formCreators[2] = &Intern::createPresidentialPardonForm;
	
	for (int i = 0; i < 3; i++)
	{
		if (formName == formTypes[i])
		{
			std::cout << "Intern creates " << formName << std::endl;
			return (this->*formCreators[i])(formTarget);
		}
	}
	
	std::cout << "Intern cannot create form: " << formName << " (unknown form type)" << std::endl;
	return NULL;
}
