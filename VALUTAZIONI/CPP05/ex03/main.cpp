/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcapalbo <mcapalbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:04:17 by mcapalbo          #+#    #+#             */
/*   Updated: 2025/03/28 15:20:56 by mcapalbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "Intern.hpp"

int	main()
{
	try
	{
		Intern	someRandomIntern;
		Bureaucrat	first("Mimmo",5);
		Bureaucrat	second("Camimmo", 10);
		Bureaucrat	third("Smimmo", 150);

		AForm	*rrf(someRandomIntern.makeForm("shrubbery creation", "tree"));
		std::cout<< "Testing" << std::endl;
		first.signForm(*rrf);
		first.executeForm(*rrf);
		delete rrf;

		
		AForm	*rrf2(someRandomIntern.makeForm("robotomy request", "robot"));
		std::cout<< "Testing" << std::endl;
		second.signForm(*rrf2);
		second.executeForm(*rrf2);
		delete rrf2;

		
		AForm	*rrf3(someRandomIntern.makeForm("shrubbery creation", "tree"));
		std::cout<< "Exception testing" << std::endl;
		third.signForm(*rrf3);
		third.executeForm(*rrf3);
		delete rrf3;
	}
	catch (const std::exception& e)
	{
		std::cout<< e.what() << std::endl;
	}
	return (0);
}