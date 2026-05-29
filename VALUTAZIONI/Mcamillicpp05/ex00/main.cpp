#include "Bureaucrat.hpp"

int main()
{
	Bureaucrat	a("FOO", 4);
	Bureaucrat	b("BAR", 148);
	Bureaucrat	d("cioa", 148);
	std::cout << d.getName() << std::endl;
	d = b;
	std::cout << d.getName() << std::endl;
	a.Promotion();
	a.Promotion();
	a.Promotion();
	b.Demotion();
	b.Demotion();
	Bureaucrat  c("Test", 150);
	try
	{
		a.Promotion();
	}
	catch (const std::exception &e) // const assicura what sia chiamato correttamente what, funziona anche senz ma é una best bractice, se ci fossero stati piu metodi in exeption vrei potuto modificarli
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	try
	{
		b.Demotion();
	}
	catch (const  std::exception &e) //exception è la classe base di tutte le eccezioni, catch prende tutte le eccezioni che non sono state catturate prima
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	try
	{
		Bureaucrat  d("Pippo", 0);
	}
	catch (const Bureaucrat::GradeTooHighException& e) // prende solo gli errori di classe ¨gradetoolow¨
	{
		std::cerr << "Exception grade too high catched: Pippo " << e.what() << std::endl;
	}
	try
	{
		c.Demotion();
	}
	catch (const Bureaucrat::GradeTooHighException& e)
	{
		std::cerr << "Exception grade too high catched: " << c.getName() << " " << e.what() << std::endl;
	}
	catch (const Bureaucrat::GradeTooLowException& e)
	{
		std::cerr << "Exception grade too low catched: " << c.getName() << " " << e.what() << std::endl;
	}

	try {
		a.Promotion();
	} catch (const std::exception &e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	return (0);
}
