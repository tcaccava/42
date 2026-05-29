/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcapalbo <mcapalbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:25:07 by mcapalbo          #+#    #+#             */
/*   Updated: 2025/03/27 13:37:17 by mcapalbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bureaucrat.hpp"

int main()
{
	try
	{
		Bureaucrat highRankBureaucrat("Gianpiergiorgio", 2);  // Bureaucrat di grado alto
		Bureaucrat midRankBureaucrat("Mimmocamimmo", 75);     // Bureaucrat di grado medio
		std::cout << highRankBureaucrat << std::endl;
		std::cout << midRankBureaucrat << std::endl;
		highRankBureaucrat.incrementGrade();  
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	try
	{
		Bureaucrat invalidBureaucrat("Pierino", -151);  // Tentativo di creare un Bureaucrat con grado non valido
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	try
	{
		Bureaucrat nearMaxGradeBureaucrat("Gianluigi", 2);  // Bureaucrat vicino al grado massimo
		std::cout << nearMaxGradeBureaucrat << std::endl;
		nearMaxGradeBureaucrat.incrementGrade();
		std::cout << nearMaxGradeBureaucrat << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	try
	{
		Bureaucrat nearMinGradeBureaucrat("Ugo", 149);  // Bureaucrat vicino al grado minimo
		std::cout << nearMinGradeBureaucrat << std::endl;
		nearMinGradeBureaucrat.decrementGrade();
		std::cout << nearMinGradeBureaucrat << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
