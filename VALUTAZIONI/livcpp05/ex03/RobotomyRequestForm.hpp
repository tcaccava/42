#pragma once

#include "AForm.hpp"

class RobotomyRequestForm : public AForm
{
	public:
		RobotomyRequestForm(void);
		RobotomyRequestForm(std::string target);
		RobotomyRequestForm(const RobotomyRequestForm& ref);
		RobotomyRequestForm& operator = (const RobotomyRequestForm& ref);
		~RobotomyRequestForm(void);
		void	execute(Bureaucrat const & executor) const;
};
