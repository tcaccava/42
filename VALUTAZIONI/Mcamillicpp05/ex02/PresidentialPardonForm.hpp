#pragma once

#ifndef PresidentialPardonForm_HPP
	#define PresidentialPardonForm_HPP

#include "AForm.hpp"

class AForm;

class PresidentialPardonForm : public AForm
{
	private:
		std::string _target;
	public:
		PresidentialPardonForm(std::string target);
		virtual void execute(Bureaucrat const &executor) const;
};

#endif