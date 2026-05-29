#include "Bureaucrat.hpp"
#include "Form.hpp"

Bureaucrat::Bureaucrat(): _name("default") , _grade(150)
{
	std::cout << "Default constructor called" << std::endl;
}

Bureaucrat::Bureaucrat(std::string nameAdd, int grade): _name(nameAdd), _grade(grade)
{
	if (grade > 150)
		throw GradeTooLowException();
	if (grade <= 0)
		throw GradeTooHighException();
	std::cout << "Parameter constructor called" << std::endl;
}

Bureaucrat::Bureaucrat(Bureaucrat &copy) : _name(copy._name), _grade(copy._grade)
{
	std::cout << "Copy constructor called" << std::endl;
}

//throw find a issue, he give it to the exception
// try/ catch try to find a solution for the issue
Bureaucrat::~Bureaucrat()
{
	std::cout << "Destructor called" << std::endl;
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

const std::string& Bureaucrat::getName()const
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
        throw GradeTooHighException();  // Si c'est impossible, je refuse
    _grade--;  // Seulement si c'est sûr, je modifie
}
void Bureaucrat::decrementGrade()
{
	if (_grade >= 150)
		throw GradeTooLowException();
	_grade++;
}

void Bureaucrat::signForm(Form &formular)
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
	// if (formular.isSigned())
	// 	std::cout << _name << " signed " << formular.getName() << std::endl;
	// else
	// 	std::cout << _name << " couldn't sign " << formular.getName() << "because" << " grade to low or juste he can't!!" << std::endl;
}
