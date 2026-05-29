#include "Form.hpp"
#include "Bureaucrat.hpp"

Form::Form() : _name("NULL"), _sign(0), _sign_grade(5), _exec_grade(5) {}

Form::Form(std::string name, const unsigned int sign_grade, const unsigned int exec_grade) : _name(name), _sign(false), _sign_grade(sign_grade), _exec_grade(exec_grade)
{
    if (sign_grade < 1 || exec_grade < 1)
        throw GradeTooHighException();
    if (sign_grade > 150 || exec_grade > 150)
        throw GradeTooLowException();
}

Form::Form(const Form& other) : _name(other._name), _sign(other._sign), _sign_grade(other._sign_grade), _exec_grade(other._exec_grade) {}

Form &Form::operator=(const Form& other)
{
    if (this != &other)
        this->_sign = other._sign;
    return(*this);
}

Form::~Form() {}


// Getters
std::string Form::getName() const
{
    return(_name);
}

bool Form::isSigned() const
{
    return(_sign);
}

unsigned int Form::getSignGrade() const
{
    return(_sign_grade);
}

unsigned int Form::getExecGrade() const
{
    return(_exec_grade);
}

// end

const char* Form::GradeTooHighException::what() const throw()
{
    return "Grade too high :(";
}

const char* Form::GradeTooLowException::what() const throw()
{
    return "Grade too low :(";
}

std::ostream &operator<<(std::ostream &os, const Form& other)
{
    std::string sig;

    if (other.isSigned())
        sig = "Signed\n";
    else
        sig = "Not Signed\n";

    os << "Form: " << other.getName() << '\n' << sig << "Sign Grade: " << other.getSignGrade() << '\n' << "Exec Grade: " << other.getExecGrade() << std::endl;
    return(os);
}

void Form::beSigned(const Bureaucrat& other)
{
    if (other.getGrade() <= this->getSignGrade())
        this->_sign = true;
    else
        throw Form::GradeTooLowException();
}
