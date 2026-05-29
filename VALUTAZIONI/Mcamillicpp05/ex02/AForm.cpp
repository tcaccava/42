#include "AForm.hpp"

AForm::AForm(std::string name, int sign, int exe) : _name(name) , _signed(false), _sign_grade(sign), _exe_grade(exe)
{
	if (this->_exe_grade > 150 || this->_sign_grade > 150)
		throw (GradeTooLowException());
	if (this->_exe_grade < 1 || this->_sign_grade < 1)
		throw(GradeTooHighException());
}
const char *AForm::GradeTooLowException::what() const throw()
{
	return ("Grade too low\n");
}

const char *AForm::GradeTooHighException::what() const throw()
{
	return ("Grade too high\n");
}

const char* AForm::NotSignedException::what() const throw()
{
	return("Form not Signed!\n");
}

const char* AForm::RobotomyFailed::what() const throw()
{
	return("Robotomy request failed, now you're a poufiasse\n");
}

AForm::~AForm()
{
}

AForm::AForm(const AForm &obj) :  _name(obj._name) , _signed(false), _sign_grade(obj._sign_grade), _exe_grade(obj._exe_grade)
{
}


AForm &AForm::operator =(const AForm &obj)
{
	this->_signed = obj._signed;
	return *this;
}

std::string AForm::getName() const
{
	return (this->_name);
}

bool AForm::getIsSigned() const
{
	return (this->_signed);
}

int AForm::getSignGrade() const
{
	return (this->_sign_grade);
}

int AForm::getExeGrade() const
{
	return (this->_exe_grade);
}

void AForm::beSigned(const Bureaucrat &ins)
{
	if (ins.getGrade() <= this->_sign_grade)
		this->_signed = true;
	else
		throw(GradeTooLowException());
}
bool AForm::executable(const Bureaucrat &ins) const
{
	if (ins.getGrade() < this->_exe_grade)
		return(false);
	return(true);
}

std::ostream& operator << (std::ostream &out, AForm const &ins)
{
	out << ins.getName() << ",  grade to exec: " << ins.getExeGrade() <<  ", signed:  " << ins.getIsSigned() <<  ",  grade to execute: " << ins.getSignGrade() << std::endl;
	return out;
}
