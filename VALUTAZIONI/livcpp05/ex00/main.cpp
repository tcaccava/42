#include "Bureaucrat.hpp"

int main()
{
	try
	{
		Bureaucrat first("Bureaucrat", 42);
		Bureaucrat second("Worst Bureaucrat", 150);
		Bureaucrat third("Best Bureaucrat", 1);
		std::cout << first << std::endl;
		std::cout << second << std::endl;
		std::cout << third << std::endl;
		std::cout << "VALID INCREMENT\n\n";
		first.IncrementGrade();
		std::cout << first << std::endl;
		std::cout << "\nINVALID DECREMENT\n\n";
		second.DecrementGrade();
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		Bureaucrat second("Worst Bureaucrat", 150);
		Bureaucrat third("Best Bureaucrat", 1);

		std::cout << second << std::endl;
		std::cout << "\nINVALID INCREMENT\n\n";
		third.IncrementGrade();
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		std::cout << "\nINVALID CREATION\n\n";
		Bureaucrat invalid("invalid", 151);
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
