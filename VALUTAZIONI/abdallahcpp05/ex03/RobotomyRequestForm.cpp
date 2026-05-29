#include "RobotomyRequestForm.hpp"
#include <fstream> 
#include <stdlib.h> 

RobotomyRequestForm::RobotomyRequestForm() : AForm("Robotomy Request", 72, 45), _target("default")
{
}
RobotomyRequestForm::RobotomyRequestForm(std::string target) :  AForm("Robotomy Request", 72, 45), _target(target)
{
}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm &copy) : AForm(copy), _target(copy._target)
{
}

RobotomyRequestForm::~RobotomyRequestForm()
{
}

void RobotomyRequestForm::execute(Bureaucrat const &executor) const
{
	int rand_value;

	rand_value = rand() % 2;
	if (!this->isSigned())
		throw std::runtime_error("Form not signed!");
	if (executor.getGrade() > this->getGradeToExecute())
		throw GradeTooLowException();
	std::cout << "BZZZZZZ! *drilling noises*" << std::endl;
	if (rand_value == 1)
	{
		std::cout << _target << " has been robotomized successfully!" << std::endl;
	}
	else
	{
	std::cout << "Robotomy failed on " << _target << "!" << std::endl;
	}
}

