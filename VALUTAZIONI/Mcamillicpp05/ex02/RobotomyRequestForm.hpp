#pragma once

#ifndef RobotomyRequestForm_HPP
	#define RobotomyRequestForm_HPP

#include "AForm.hpp"

class AForm;

class RobotomyRequestForm : public AForm
{
	private:
		std::string _target;
	public:
		RobotomyRequestForm(std::string target);
		virtual void execute(Bureaucrat const &executor) const;
};

#endif