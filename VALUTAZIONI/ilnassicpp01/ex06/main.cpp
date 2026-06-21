/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 16:15:29 by ilnassi           #+#    #+#             */
/*   Updated: 2026/06/08 16:15:33 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Harl.hpp"
#include <iostream>

int	main(int ac, char* av[])
{
	Harl harl;

	if (ac != 2)
	{
		std::cout << "[ Probably complaining about insignificant problems ]\n";
		return 0;
	}

	harl.complain(av[1]);
	return 0;
}
