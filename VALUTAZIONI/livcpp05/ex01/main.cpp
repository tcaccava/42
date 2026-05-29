#include "Bureaucrat.hpp"
#include "Form.hpp"

int main()
{
	try
	{
		Form		Form("first", 50, 70);
		std::cout << Form << std::endl;
		Bureaucrat first("Liv", 42);
		first.signForm(Form);
		std::cout << Form << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	try
	{
		Form Form("second", 50, 70);
		Bureaucrat second("Worst Bureaucrat", 150);
		std::cout << "\nLOW VOTE\n\n";
		second.signForm(Form);
		std::cout << Form << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	try
	{
		std::cout << "\nINVALID CREATION\n\n";
		Form invalid("invalid", -42, 150);
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
