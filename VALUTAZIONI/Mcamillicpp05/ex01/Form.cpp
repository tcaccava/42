#include "Form.hpp"

Form::Form(std::string name, int sign, int exe) : _name(name) , _signed(false), _sign_grade(sign), _exe_grade(exe)
{
	if (this->_exe_grade > 150 || this->_sign_grade > 150)
		throw (GradeTooLowException());
	if (this->_exe_grade < 1 || this->_sign_grade < 1)
		throw(GradeTooHighException());
}
const char *Form::GradeTooLowException::what() const throw()
{
	return ("Grade too low\n");
}

const char *Form::GradeTooHighException::what() const throw()
{
	return ("Grade too high\n");
}

Form::~Form()
{
}

Form::Form(const Form &obj) :  _name(obj._name) , _signed(obj._signed), _sign_grade(obj._sign_grade), _exe_grade(obj._exe_grade)
{
}


Form &Form::operator =(const Form &obj)
{
	this->_signed = obj._signed;
	return *this;
}

std::string Form::getName() const
{
	return (this->_name);
}

bool Form::getIsSigned() const
{
	return (this->_signed);
}

int Form::getSignGrade() const
{
	return (this->_sign_grade);
}

int Form::getExeGrade() const
{
	return (this->_exe_grade);
}

void Form::beSigned(const Bureaucrat &ins)
{
	if (ins.getGrade() <= this->_sign_grade)
		this->_signed = true;
	else
		throw(GradeTooLowException());
}

std::ostream& operator << (std::ostream &out, Form const &ins)
{
	out << ins.getName() << ",  grade to exec: " << ins.getExeGrade() <<  ", signed:  " << ins.getIsSigned() <<  ",  grade to execute: " << ins.getSignGrade() << std::endl;
	return out;
}