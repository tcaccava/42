#include "AForm.hpp"

AForm::AForm(const std::string _name, const int to_sign, const int to_exec, const std::string &_target) : name(_name), to_sign(to_sign), to_exec(to_exec), target(_target)
{
    if (to_sign < 1)
        throw GradeTooHighException();
    if (to_sign > 150)
        throw GradeTooLowException();
    if (to_exec < 1)
        throw GradeTooHighException();
    if (to_exec > 150)
        throw GradeTooLowException();
    is_signed = false;
}
AForm::~AForm() {}

AForm::AForm(const AForm &other) : name(other.name), to_sign(other.to_sign), to_exec(other.to_exec)
{
    this->is_signed = other.is_signed;
}
AForm &AForm::operator=(const AForm &other)
{
    if (this != &other)
        this->is_signed = other.is_signed;
    return *this;
}

std::string AForm::getName() const
{
    return name;
}

bool AForm::isSigned() const
{
    return is_signed;
}

int AForm::getGradeToSign() const
{
    return to_sign;
}

int AForm::getGradeToExec() const
{
    return to_exec;
}

std::string AForm::getTarget() const {
    return target;
}

void AForm::beSigned(const Bureaucrat &burocrat)
{
    if (burocrat.getGrade() <= to_sign)
        this->setSigned();
    else
        throw GradeTooLowException();
}

void AForm::setSigned()
{
    is_signed = true;
}
const char *AForm::GradeTooHighException::what() const throw()
{
    return ("grade is too high.");
}

const char *AForm::GradeTooLowException::what() const throw()
{
    return ("grade is too low.");
}

std::ostream &operator<<(std::ostream &os, const AForm &AForm)
{
    os << AForm.getName() << " , grade to sign " << AForm.getGradeToSign() << " , grade to exec " << AForm.getGradeToExec() << ".";
    return os;
}

const char* AForm::NotSignedException::what() const throw() {
    return "the form is not signed.";
} 

void AForm::execute(const Bureaucrat &executor) const
{
    if (isSigned() == false)
        throw NotSignedException();
    if (executor.getGrade() > getGradeToExec())
        throw GradeTooLowException();
    if ((isSigned() == true) && executor.getGrade() <= getGradeToExec()) {
        action();
    }
}