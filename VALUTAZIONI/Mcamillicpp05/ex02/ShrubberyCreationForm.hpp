#pragma once

#ifndef ShrubberyCreationForm_HPP
	#define ShrubberyCreationForm_HPP

#include "AForm.hpp"
#include <fstream>

class AForm;

class ShrubberyCreationForm : public AForm
{
	private:
		std::string _target;
	public:
		ShrubberyCreationForm(std::string target);
		virtual void execute(Bureaucrat const &executor) const;
};

#endif