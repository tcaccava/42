#include "Bureaucrat.hpp"

Bureaucrat::Bureaucrat(): _name("default") , _grade(150)
{
	//std::cout << "Default constructor called Bureaucrat" << std::endl;
}

Bureaucrat::Bureaucrat(std::string nameAdd, int grade): _name(nameAdd), _grade(grade)
{
	if (grade > 150)
		throw GradeTooLowException();
	if (grade <= 0)
		throw GradeTooHighException();
	//std::cout << "Parameter constructor called Bureaucrat" << std::endl;
}

Bureaucrat::Bureaucrat(Bureaucrat &copy) : _name(copy._name), _grade(copy._grade)
{
	//std::cout << "Copy constructor called Bureaucrat" << std::endl;
}

//throw find a issue, he give it to the exception
// try/ catch try to find a solution for the issue
Bureaucrat::~Bureaucrat()
{
	//std::cout << "Destructor called Bureaucrat" << std::endl;
}

Bureaucrat &Bureaucrat::operator=(Bureaucrat &other)
{
	if (this != &other)
	{
		//_name = other._name;
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
		std::cout <<  _name << " couldn't sign " << formular.getName() << " because " << except.what() << std::endl;
	}
}
