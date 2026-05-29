#include "Bureaucrat.hpp"

Bureaucrat::Bureaucrat(): _name("default") , _grade(150)
{
}

Bureaucrat::Bureaucrat(std::string nameAdd, int grade): _name(nameAdd), _grade(grade)
{
	if (grade > 150)
		throw GradeTooLowException();
	if (grade <= 0)
		throw GradeTooHighException();
}

Bureaucrat::Bureaucrat(Bureaucrat &copy) : _name(copy._name), _grade(copy._grade)
{
}


Bureaucrat::~Bureaucrat()
{
}

Bureaucrat &Bureaucrat::operator=(Bureaucrat &other)
{
	if (this != &other)
	{
		_grade = other._grade;
	}
	return (*this);
}

const std::string Bureaucrat::getName()const
{
	return (_name);
}

int Bureaucrat::getGrade()const
{
	return (_grade);
}

void Bureaucrat::incrementGrade()
{
    if (_grade <= 1)
        throw GradeTooHighException();
    _grade--;
}
void Bureaucrat::decrementGrade()
{
	if (_grade >= 150)
		throw GradeTooLowException();
	_grade++;
}

void Bureaucrat::signForm(AForm &formular)
{
	try
	{
		formular.beSigned(*this);
		std::cout << _name << " signed " << formular.getName() << std::endl;
	}
	catch(std::exception &except)
	{
		std::cout <<  _name << " couldn't sign " << formular.getName() << " because " << except.what() << std::endl;
	}
}

void Bureaucrat::executeForm(AForm const &formular)
{
	try 
	{
		formular.execute(*this);
		std::cout << _name << " executed " << formular.getName() << std::endl;
	}
	catch(std::exception &except)
	{
		std::cout <<  _name << " couldn't execute " << formular.getName() << " because " << except.what() << std::endl;
	}
}
