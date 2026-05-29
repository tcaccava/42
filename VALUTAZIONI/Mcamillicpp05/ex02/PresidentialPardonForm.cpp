#include "PresidentialPardonForm.hpp"


PresidentialPardonForm::PresidentialPardonForm(std::string target) : AForm("PresidentialPardonForm", 25, 5),  _target(target)
{

}
void PresidentialPardonForm::execute(const Bureaucrat &executor) const
{
	if (this->getIsSigned() == false)
		throw(NotSignedException());
	if (!this->executable(executor))
		throw(GradeTooLowException());
	std::cout<<"mr "<< this->_target <<" has been pardoned by Zaphod Beeblebrox"<<std::endl;
}

