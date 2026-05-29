#include "Form.hpp"

Form::Form(std::string const &name, int gradeToSign, int gradeToExec): _name(name), _gradeToSign(gradeToSign), _gradeToExec(gradeToExec)
{
    _signed = false;
    
    if (_gradeToSign > 150 || _gradeToExec > 150)
        throw GradeTooLowException();
    if (_gradeToSign < 1 || _gradeToExec < 1)
        throw GradeTooHighException();
    std::cout << *this << std::endl;
}

Form::~Form()
{

}

Form::Form(Form const &src):  _name(src.getName()), _gradeToSign(src.getGradeToSign()), _gradeToExec(src.getGradeToExec())
{
    _signed = src.getSigned();
}

Form& Form::operator=(Form const &src)
{
    if (this != &src)
        _signed = src._signed;
    return (*this);
}

std::string const &Form::getName() const
{
    return _name;
}

bool Form::getSigned() const
{
    return _signed;
}

int Form::getGradeToSign() const
{
    return _gradeToSign;
}

int Form::getGradeToExec() const
{
    return _gradeToExec;
}

std::ostream &operator<<(std::ostream &os, Form const &src)
{
    os << "INFO ABOUT Form:" << std::endl;
    os << "- name: " << src.getName() << std::endl;
    os << "- grade to sign: " << src.getGradeToSign() << std::endl;
    os << "- grade to exec: " << src.getGradeToExec() << std::endl;
    os << "- signed: ";
    if (src.getSigned())
        os << "yes" << std::endl;
    else
        os << "no" << std::endl;
    return os;
}

const char *Form::GradeTooLowException::what() const throw()
{
    return "I'm in Form -> Grade too low";
}

const char *Form::GradeTooHighException::what() const throw()
{
    return "I'm in Form -> Grade too high";
}

void  Form::beSigned(Bureaucrat& bureaucrat)
{

    if (bureaucrat.getGrade() <= this->getGradeToSign())
        this->_signed = true;
    else
        throw GradeTooLowException();
}

