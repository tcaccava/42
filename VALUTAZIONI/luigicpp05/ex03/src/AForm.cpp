#include "AForm.hpp"
#include <unistd.h>   //for sleep

AForm::AForm(std::string const &name, int gradeToSign, int gradeToExec): _name(name), _gradeToSign(gradeToSign), _gradeToExec(gradeToExec)
{
    _signed = false;
    std::cout << *this << std::endl;

    if (_gradeToSign > 150 || _gradeToExec > 150)
        throw GradeTooLowException();
    if (_gradeToSign < 1 || _gradeToExec < 1)
        throw GradeTooHighException();
}

AForm::~AForm(){}

AForm::AForm(AForm const &src):  _name(src.getName()), _gradeToSign(src.getGradeToSign()), _gradeToExec(src.getGradeToExec())
{
    _signed = src.getSigned();
}

AForm& AForm::operator=(AForm const &src)
{
    if (this != &src)
        _signed = src._signed;
    return (*this);
}

std::string const &AForm::getName() const
{
    return _name;
}

bool AForm::getSigned() const
{
    return _signed;
}

int AForm::getGradeToSign() const
{
    return _gradeToSign;
}

int AForm::getGradeToExec() const
{
    return _gradeToExec;
}

std::ostream &operator<<(std::ostream &os, AForm const &src)
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

const char *AForm::GradeTooLowException::what() const throw()
{
    return "Grade too low";
}

const char *AForm::GradeTooHighException::what() const throw()
{
    return "Grade too high";
}

void  AForm::beSigned(Bureaucrat const& bureaucrat)
{
    std::cout << "Bureaucrat " << bureaucrat.getName() << ": signing in progress ...\n" << std::flush;;
    sleep(1);

    if (bureaucrat.getGrade() <= this->getGradeToSign())
    {
        this->_signed = true;
        std::cout << "The bureucrat named " << bureaucrat.getName() << " has signed the form\n";
    }
    else
        throw GradeTooLowException();
}


