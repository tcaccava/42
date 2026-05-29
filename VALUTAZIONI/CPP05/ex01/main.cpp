/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcapalbo <mcapalbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:14:23 by mcapalbo          #+#    #+#             */
/*   Updated: 2025/03/27 15:27:47 by mcapalbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bureaucrat.hpp"
#include "Form.hpp"

int main()
{
	try
	{
		Bureaucrat highRankBureaucrat("Gianpiergiorgio", 1);
		Bureaucrat lowRankBureaucrat("Mimmocamimmo", 150);
		Form importantForm("ImportantForm", 1, 1);
		Form regularForm("RegularForm", 150, 150);

		std::cout << highRankBureaucrat << std::endl;
		std::cout << lowRankBureaucrat << std::endl;
		std::cout << importantForm << std::endl;
		std::cout << regularForm << std::endl;

		highRankBureaucrat.signForm(importantForm);
		lowRankBureaucrat.signForm(regularForm);
		lowRankBureaucrat.signForm(importantForm);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	try
	{
		Form invalidForm1("InvalidForm1", 151, 1);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	try
	{
		Form invalidForm2("InvalidForm2", 1, 0);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
