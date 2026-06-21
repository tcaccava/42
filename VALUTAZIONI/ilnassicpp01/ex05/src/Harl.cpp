/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 15:38:30 by ilnassi           #+#    #+#             */
/*   Updated: 2026/06/08 15:44:20 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Harl.hpp"

void	Harl::debug(void)
{
	std::cout << "[ DEBUG ]\n";
	std::cout << "I love having extra bacon for my 7XL-double-cheese-"
		<< "triple-pickle-special-ketchup burger. I really do!\n\n";
}

void	Harl::info(void)
{
	std::cout << "[ INFO ]\n";
	std::cout << "I cannot believe adding extra bacon costs more money. "
		<< "You didn't put enough bacon in my burger! "
		<< "If you did, I wouldn't be asking for more!\n\n";
}

void	Harl::warning(void)
{
	std::cout << "[ WARNING ]\n";
	std::cout << "I think I deserve to have some extra bacon for free. "
		<< "I've been coming for years, whereas you started "
		<< "working here just last month.\n\n";
}

void	Harl::error(void)
{
	std::cout << "[ ERROR ]\n";
	std::cout << "This is unacceptable! "
		<< "I want to speak to the manager now.\n\n";
}

void	Harl::complain(const std::string& level)
{
	void (Harl::*funcs[4])(void) = 
	{
		&Harl::debug,
		&Harl::info,
		&Harl::warning,
		&Harl::error
	};
	
	std::string levels[4] = {"DEBUG", "INFO", "WARNING", "ERROR"};
	
	for (int i = 0; i < 4; i++) 
	{
		if (levels[i] == level)
		{
			(this->*funcs[i])();
			return;
		}
	}
	std::cout << "[ Unknown level ]\n" << level << " is not a valid level.\n\n";
}
