#ifndef INTERN_HPP
#define INTERN_HPP

#include <iostream>
#include "AForm.hpp"

class Intern
{
	public:
		Intern();
		Intern(Intern &copy);
		virtual ~Intern();
		Intern &operator=(const Intern &other);

		AForm *makeForm(const std::string formName, const std::string formTarget);

	private:
		AForm *createShrubberyCreationForm(const std::string &target);
		AForm *createRobotomyRequestForm(const std::string &target);
		AForm *createPresidentialPardonForm(const std::string &target);
};

#endif
