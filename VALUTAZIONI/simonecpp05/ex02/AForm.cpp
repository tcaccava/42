#include "AForm.hpp"
#include "Bureaucrat.hpp"

AForm::AForm() : _name("Default"), _sign(false), _sign_grade(150), _exec_grade(150)
{
}

AForm::AForm(std::string name, const unsigned int sign_grade, const unsigned int exec_grade)
    : _name(name), _sign(false), _sign_grade(sign_grade), _exec_grade(exec_grade)
{
    if (sign_grade < 1 || exec_grade < 1)
        throw GradeTooHighException();
    if (sign_grade > 150 || exec_grade > 150)
        throw GradeTooLowException();
}

AForm::AForm(const AForm& other)
    : _name(other._name), _sign(other._sign), _sign_grade(other._sign_grade), _exec_grade(other._exec_grade)
{
}

AForm& AForm::operator=(const AForm& other)
{
    if (this != &other)
        _sign = other._sign;
    return *this;
}

AForm::~AForm()
{
}

const char* AForm::GradeTooHighException::what() const throw()
{
    return "Grade too high";
}

const char* AForm::GradeTooLowException::what() const throw()
{
    return "Grade too low";
}

const char* AForm::FormNotSignedException::what() const throw()
{
    return "Form not signed";
}

void AForm::beSigned(const Bureaucrat& other)
{
    if (other.getGrade() <= _sign_grade)
        _sign = true;
    else
        throw GradeTooLowException();
}

void AForm::execute(Bureaucrat const & executor) const
{
    if (!_sign)
        throw FormNotSignedException();
    if (executor.getGrade() > _exec_grade)
        throw GradeTooLowException();
    executeAction();
}

std::string AForm::getName() const { return _name; }
bool AForm::isSigned() const { return _sign; }
unsigned int AForm::getSignGrade() const { return _sign_grade; }
unsigned int AForm::getExecGrade() const { return _exec_grade; }

std::ostream &operator<<(std::ostream &os, const AForm& other)
{
    os << "Form: " << other.getName()
       << ", Signed: " << (other.isSigned() ? "yes" : "no")
       << ", Sign grade: " << other.getSignGrade()
       << ", Exec grade: " << other.getExecGrade();
    return os;
}
