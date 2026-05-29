#include "PresidentialPardonForm.hpp"

PresidentialPardonForm::PresidentialPardonForm(void)  : AForm()
{}

PresidentialPardonForm::PresidentialPardonForm(std::string target) : AForm(target, 25, 5)
{}

PresidentialPardonForm::PresidentialPardonForm(const PresidentialPardonForm& ref) : AForm(ref)
{}

PresidentialPardonForm& PresidentialPardonForm::operator = (const PresidentialPardonForm& ref)
{
	(void) ref;
	return (*this);
}

PresidentialPardonForm::~PresidentialPardonForm(void)
{}

void PresidentialPardonForm::execute(Bureaucrat const& executor) const
{
	try
	{
		beExecuted(executor);
		std::cout << getName() << " has been pardoned by Zaphod Beeblebrox." << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << executor.getName() << " couldn't execute " << getName() << " because " << e.what() << std::endl;
	}
}