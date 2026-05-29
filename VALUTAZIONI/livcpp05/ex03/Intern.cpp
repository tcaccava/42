#include "Intern.hpp"

Intern::Intern(void) {}

Intern::Intern(const Intern& ref)
{
	*this = ref;
}

Intern& Intern::operator = (const Intern& ref)
{
	(void) ref;
	return (*this);
}

Intern::~Intern(void) {}

AForm* Intern::makeForm(std::string type, std::string target)
{
	AForm* functions[3];
	AForm* object = NULL;
	std::string types[3] = {"shrubbery request", "robotomy request", "presidential request"};

	functions[0] = new ShrubberyCreationForm(target);
	functions[1] = new RobotomyRequestForm(target);
	functions[2] = new PresidentialPardonForm(target);

	for (int i = 0; i < 3; i++)
	{
		if (type == types[i])
		{
			std::cout << "Intern creates " << target << std::endl;
			object = functions[i];
		}
		else
			delete functions[i];
	}
	if (!object)
		throw std::runtime_error("type not recognized");
	return (object);
}