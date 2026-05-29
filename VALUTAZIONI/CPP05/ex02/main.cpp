/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcapalbo <mcapalbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:38:16 by mcapalbo          #+#    #+#             */
/*   Updated: 2025/03/28 14:30:00 by mcapalbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main()
{
	try
	{
		Bureaucrat bob("Mimmo", 1);
		ShrubberyCreationForm shrubbery("home");
		RobotomyRequestForm robotomy("Camimmo");
		PresidentialPardonForm pardon("Stirlo");

		bob.signForm(shrubbery);
		bob.signForm(robotomy);
		bob.signForm(pardon);

		bob.executeForm(shrubbery);
		bob.executeForm(robotomy);
		bob.executeForm(pardon);
	}
	catch (std::exception &e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}