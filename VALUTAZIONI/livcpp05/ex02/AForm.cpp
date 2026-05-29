#include "AForm.hpp"

AForm::AForm(void) : _signed(false), _name("anonymous Form"), _gradeToSign(42), _gradeToExecute(42)
{}

AForm::AForm(std::string name, const int gradeToSign, const int gradeToExecute) : _signed(false), _name(name), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	if (_gradeToExecute > 150 || _gradeToSign > 150)
		throw AForm::GradeTooLowException();
	if (_gradeToExecute < 1 || _gradeToSign < 1)
		throw AForm::GradeTooHighException();
}

AForm::AForm(const AForm& ref) : _signed(false), _name(ref._name), _gradeToSign(ref._gradeToSign), _gradeToExecute(ref._gradeToExecute)
{}

AForm& AForm::operator = (const AForm& ref)
{
	(void) ref;
	return (*this);
}

AForm::~AForm(void){}

std::string AForm::getName(void) const
{
	return (this->_name);
}

int AForm::getGradeToSign(void) const
{
	return (this->_gradeToSign);
}

int AForm::getGradeToExecute(void) const
{
	return (this->_gradeToExecute);
}

bool	AForm::isItSigned(void) const
{
	return (this->_signed);
}

void	AForm::beSigned(const Bureaucrat& bureaucrat)
{
	if (bureaucrat.getGrade() <= this->_gradeToSign)
		_signed = true;
	else
		throw AForm::GradeTooLowException();
}

bool	AForm::beExecuted(const Bureaucrat& bureaucrat) const
{
	if (bureaucrat.getGrade() <= this->_gradeToExecute)
		return true;
	else
		throw GradeTooLowException();
}

const char*	AForm::GradeTooLowException::what() const throw()
{
	return ("Grade Too low..");
}

const char*	AForm::GradeTooHighException::what() const throw()
{
	return ("Grade too high..");
}

std::ostream& operator<< (std::ostream& os, const AForm& ref)
{
	os << "Form " << ref.getName() << " needs a grade of " << ref.getGradeToSign();
	os << " to be signed and a grade of " << ref.getGradeToExecute();
	os << " to be executed" << std::endl;
	if (ref.isItSigned() == true)
		os << "Current state: Form signed";
	else
		os << "Current state: to sign";
	return (os);
}
