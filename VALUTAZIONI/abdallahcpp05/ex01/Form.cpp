#include "Form.hpp"
#include "Bureaucrat.hpp"

Form::Form(): _name("default") , _signed(false) , _gradeToSign(150), _gradeToExecute(150)
{
	std::cout << "Default constructor called" << std::endl;
}

Form::Form(const Form &copy): _name(copy._name) , _signed(copy._signed), _gradeToSign(copy._gradeToSign) , _gradeToExecute(copy._gradeToExecute)
{
	std::cout << "Copy constructor called" << std::endl;
}

Form::Form(const std::string &name, int gradeToSign, int gradeToExecute): _name(name), _signed(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	if (gradeToSign < 1 || gradeToExecute < 1)
		throw GradeTooHighException();
	if (gradeToSign > 150 || gradeToExecute > 150)
		throw GradeTooLowException();
	std::cout << "Parameter constructor called" << std::endl;
}
Form::~Form()
{
	std::cout << "Destructor called" << std::endl;
}

Form& Form::operator=(const Form& other)
{
	if (this != &other)
		_signed = other._signed;
	return *this;
}

const std::string Form::getName()const
{
	return (_name);
}

bool Form::isSigned()const
{
	return(_signed);
}
int Form::getGradeToExecute()const
{
	return(_gradeToExecute);
}

int Form::getGradeToSign()const
{
	return (_gradeToSign);
}

void Form::beSigned(const Bureaucrat &guy)
{
	if (guy.getGrade() <= _gradeToSign)
		_signed = true;
	else
		throw GradeTooLowException();
}

std::ostream& operator<<(std::ostream& os, const Form& form)
{
	os	<< "Form " << form.getName()
		<< ", signed: " << (form.isSigned() ? "yes" : "no")
		<< ", grade required to sign: " << form.getGradeToSign()
		<< ", grade required to execute: " << form.getGradeToExecute();
	return os;
}

