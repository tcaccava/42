#include "RobotomyRequestForm.hpp"


RobotomyRequestForm::RobotomyRequestForm(std::string target) : AForm("RobotomyRequestForm", 72, 45),  _target(target)
{

}
void RobotomyRequestForm::execute(const Bureaucrat &executor) const
{
	if (this->getIsSigned() == false)
		throw(NotSignedException());
	if (!this->executable(executor))
		throw(GradeTooLowException());
	if (rand() % 2)
		throw(RobotomyFailed());
	else
		std::cout<<"**drilling noises** get ready for the next battle, now you're Jack - 5!"<<std::endl;
}

