#include "AForm.hpp"
#include "Bureaucrat.hpp"

AForm::AForm(): _name("default") , _signed(false) , _gradeToSign(150), _gradeToExecute(150)
{
}

AForm::AForm(const AForm &copy): _name(copy._name) , _signed(copy._signed), _gradeToSign(copy._gradeToSign) , _gradeToExecute(copy._gradeToExecute)
{
}

AForm::AForm(const std::string &name, int gradeToSign, int gradeToExecute): _name(name), _signed(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	if (gradeToSign < 1 || gradeToExecute < 1)
		throw GradeTooHighException();
	if (gradeToSign > 150 || gradeToExecute > 150)
		throw GradeTooLowException();
}
AForm::~AForm()
{
}

AForm& AForm::operator=(const AForm& other)
{
	if (this != &other)
	{
		_signed = other._signed;
	}
	return *this;
}

std::string AForm::getName()const
{
	return (_name);
}

bool AForm::isSigned() const
{
	return(_signed);
}
int AForm::getGradeToExecute()const
{
	return(_gradeToExecute);
}

int AForm::getGradeToSign()const
{
	return (_gradeToSign);
}

void AForm::beSigned(const Bureaucrat &guy)
{
	if (guy.getGrade() <= _gradeToSign)
		_signed = true;
	else
		throw GradeTooLowException();
}
std::ostream& operator<<(std::ostream& os, const AForm& form)
{
	os << "Form " << form.getName() 
	   << ", signed: " << (form.isSigned() ? "yes" : "no")
	   << ", grade required to sign: " << form.getGradeToSign()
	   << ", grade required to execute: " << form.getGradeToExecute();
	return os;
}
