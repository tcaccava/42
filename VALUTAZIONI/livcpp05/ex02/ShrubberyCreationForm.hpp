#pragma once

#include "AForm.hpp"
#include <fstream>

class ShrubberyCreationForm : public AForm
{
	public:
		ShrubberyCreationForm(void);
		ShrubberyCreationForm(std::string target);
		ShrubberyCreationForm(const ShrubberyCreationForm& ref);
		ShrubberyCreationForm& operator = (const ShrubberyCreationForm& ref);
		~ShrubberyCreationForm(void);
		void	execute(Bureaucrat const & executor) const;
};