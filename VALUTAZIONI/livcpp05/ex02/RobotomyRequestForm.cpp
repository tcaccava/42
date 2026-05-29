#include "RobotomyRequestForm.hpp"

RobotomyRequestForm::RobotomyRequestForm(void)  : AForm()
{}

RobotomyRequestForm::RobotomyRequestForm(std::string target) : AForm(target, 72, 45)
{}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm& ref) : AForm(ref)
{}

RobotomyRequestForm& RobotomyRequestForm::operator = (const RobotomyRequestForm& ref)
{
	(void) ref;
	return (*this);
}

RobotomyRequestForm::~RobotomyRequestForm(void)
{}

void RobotomyRequestForm::execute(Bureaucrat const& executor) const
{
	try
	{
		beExecuted(executor);
		std::cout << "GHSUgdhjdenfurbvcjwefneqi" << std::endl;
		std::cout << getName() << " has been robotomized." << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << executor.getName() << " failed to robotomized " << getName() << " because " << e.what() << std::endl;
	}
}