#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main()
{
	try
	{
		std::cout << "SHRUBBERY\n\n";
		ShrubberyCreationForm	form("target");
		Bureaucrat prova("42", 70);
		std::cout << prova << std::endl;
		std::cout << form << std::endl << std::endl;

		prova.signForm(form);
		form.execute(prova);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	try
	{
		std::cout << "\nROBOTOMY\n\n";
		RobotomyRequestForm second("robot");
		Bureaucrat	prova("Liv", 70);

		std::cout << prova << std::endl;
		std::cout << second << std::endl << std::endl;

		prova.signForm(second);
		prova.executeForm(second);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	try
	{
		std::cout << "\nPARDON\n\n";
		PresidentialPardonForm second("robot");
		Bureaucrat	prova("Liv", 70);

		std::cout << prova << std::endl;
		std::cout << second << std::endl << std::endl;

		prova.signForm(second);
		second.execute(prova);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
