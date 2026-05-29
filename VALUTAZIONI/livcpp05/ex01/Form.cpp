#include "Form.hpp"

Form::Form(void) : _signed(false), _name("anonymous Form"), _gradeToSign(42), _gradeToExecute(42)
{}

Form::Form(std::string name, const int gradeToSign, const int gradeToExecute) : _signed(false), _name(name), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	if (_gradeToExecute > 150 || _gradeToSign > 150)
		throw Form::GradeTooLowException();
	if (_gradeToExecute < 1 || _gradeToSign < 1)
		throw Form::GradeTooHighException();
}

Form::Form(const Form& ref) : _signed(false), _name(ref._name), _gradeToSign(ref._gradeToSign), _gradeToExecute(ref._gradeToExecute)
{}

Form& Form::operator = (const Form& ref)
{
	(void) ref;
	return (*this);
}

Form::~Form(void){}

std::string Form::getName(void) const
{
	return (this->_name);
}

int Form::getGradeToSign(void) const
{
	return (this->_gradeToSign);
}

int Form::getGradeToExecute(void) const
{
	return (this->_gradeToExecute);
}

bool	Form::isItSigned(void) const
{
	return (this->_signed);
}

void	Form::beSigned(const Bureaucrat& bureaucrat)
{
	if (bureaucrat.getGrade() <= this->_gradeToSign)
		_signed = true;
	else
		throw GradeTooLowException();
}

const char*	Form::GradeTooLowException::what() const throw()
{
	return ("Grade Too low..");
}

const char*	Form::GradeTooHighException::what() const throw()
{
	return ("Grade too high..");
}

std::ostream& operator<< (std::ostream& os, const Form& ref)
{
	os << "Form " <<ref.getName() << " needs a grade of " << ref.getGradeToSign();
	os << " to be signed and a grade of " << ref.getGradeToExecute();
	os << " to be executed" << std::endl;
	if (ref.isItSigned() == true)
		os << "Current state: signed";
	else
		os << "Current state: to sign";
	return (os);
}
