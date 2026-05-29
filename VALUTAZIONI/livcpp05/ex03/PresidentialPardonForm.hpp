#pragma once

#include "AForm.hpp"

class PresidentialPardonForm : public AForm
{
	public:
		PresidentialPardonForm(void);
		PresidentialPardonForm(std::string target);
		PresidentialPardonForm(const PresidentialPardonForm& ref);
		PresidentialPardonForm& operator = (const PresidentialPardonForm& ref);
		~PresidentialPardonForm(void);
		void	execute(Bureaucrat const & executor) const;
};