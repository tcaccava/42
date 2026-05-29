#include "Bureaucrat.hpp"

Bureaucrat::Bureaucrat() {}

Bureaucrat::Bureaucrat(std::string name, unsigned int grade) : _name(name)
{
    if (grade > 150)
    {
        throw Bureaucrat::GradeTooLowException();
    }
    else if (grade < 1)
    {
        throw Bureaucrat::GradeTooHighException();
    }
    else
        _grade = grade;
}

Bureaucrat::Bureaucrat(const Bureaucrat& other) : _name(other._name), _grade(other._grade) {}

Bureaucrat &Bureaucrat::operator=(const Bureaucrat& other)
{
    if (this != &other)
        this->_grade = other._grade;
    return(*this);
}

// Bureaucrat &Bureaucrat::operator<<(const Bureaucrat& other)
// {
//     (void)other;
//     std::cout << _name << ", bureaucrat grade " << _grade << std::endl;
//     return(*this);
// }

Bureaucrat::~Bureaucrat() {}

std::string Bureaucrat::getName() const
{
    return(_name);
}

unsigned int Bureaucrat::getGrade() const
{
    return(_grade);
}

void Bureaucrat::incrementGrade()
{
    if (_grade == 1)
        throw Bureaucrat::GradeTooHighException();
    _grade -= 1;
}

void Bureaucrat::decrementGrade()
{
    if (_grade == 150)
        throw Bureaucrat::GradeTooLowException();
    _grade += 1;
}


const char* Bureaucrat::GradeTooHighException::what() const throw()
{
    return "Grade too high :(";
}

const char* Bureaucrat::GradeTooLowException::what() const throw()
{
    return "Grade too low :(";
}

std::ostream &operator<<(std::ostream &os, const Bureaucrat& other)
{
    os << other.getName() << ", bureaucrat grade " << other.getGrade() << std::endl;
    return(os);
}
