#pragma once

#include <iostream>
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"

class Intern
{
	public:
		Intern(void);
		Intern(const Intern& ref);
		Intern& operator = (const Intern& ref);
		~Intern(void);

		AForm* makeForm(std::string type, std::string target);
};