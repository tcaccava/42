#include "Bureaucrat.hpp"
#include "AForm.hpp"

Bureaucrat::Bureaucrat(){}

Bureaucrat::Bureaucrat(std::string const &name, int grade): _name(name), _grade(grade)
{
    try
    {
        if (_grade < 1)
            throw GradeTooHighException();
        else if (_grade > 150)
            throw GradeTooLowException(); 
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
        throw ;
    }
}

Bureaucrat::~Bureaucrat() {}

Bureaucrat::Bureaucrat(Bureaucrat const &src) : _name(src._name)
{
    //std::cout << "copy constructor called\n";
    _grade = src._grade;
}

Bureaucrat& Bureaucrat::operator=(Bureaucrat const &src)
{
    //std::cout << "copy assignment operator called\n";
    if (this != &src)
        _grade = src._grade;
    return (*this);
}


std::string const &Bureaucrat::getName() const
{
    return _name;
}

int Bureaucrat::getGrade() const
{
    return (_grade);
}

const char *Bureaucrat::GradeTooHighException::what() const throw()
{
    return ("grade is too high");
}

const char *Bureaucrat::GradeTooLowException::what() const throw()
{
    return ("grade is too low");
}


void Bureaucrat::incrementGrade()
{
    _grade = _grade - 1;
    try
    {
        if (_grade < 1)
            throw GradeTooHighException();
    }
    catch (GradeTooHighException& e)
    {
        std::cout << e.what() << std::endl;
        throw ;
    }
}

void Bureaucrat::decrementGrade()
{
    _grade = _grade + 1;
    try
    {
        if (_grade > 150)
        throw GradeTooLowException();
    }
    catch(GradeTooLowException& e) 
    {
        /*  
            you alwas should catch the reference 
            becouse in the try block it is created a derived block 
            (not the base class excpetion, but the derived class from the base).
            If you don't cathc the reference, a new base-class object is created 
            and so no polymorfism is guaranteed beouse of slicing that cut off your customized what().
        */
        std::cout << e.what() << std::endl;
        throw ;
    }
}

void Bureaucrat::signForm(AForm& form)
{
    try 
    {
        form.beSigned(*this);
        std::cout << this->getName() << " signed " << form.getName() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << this->getName() << " couldn’t sign " << form.getName() << " becouse " << e.what() << std::endl;
        throw ;
    }
}

std::ostream& operator<<(std::ostream& os, const Bureaucrat& src)
{
    os << src.getName() << ", bureaucrat grade " << src.getGrade() << "." << std::endl;
    return os;
}

