/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 15:36:31 by ilnassi           #+#    #+#             */
/*   Updated: 2026/06/08 15:55:16 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Harl.hpp"
#include <iostream>

int	main(int ac, char* av[])
{
	Harl harl;

	if (ac == 2)
	{
		harl.complain(av[1]);
		return 0;
	}
	else if (ac > 2)
	{
		std::cerr << "Usage: ./Harl [level]\n";
		return 1;
	}

	// Test di fallback automatici per dimostrare che Harl si lamenta molto
	std::cout << "=========================================\n";
	std::cout << "      RUNNING AUTOMATED HARL TESTS       \n";
	std::cout << "=========================================\n\n";

	harl.complain("DEBUG");
	harl.complain("INFO");
	harl.complain("WARNING");
	harl.complain("ERROR");
	
	std::cout << "Testing an invalid level:\n";
	harl.complain("GIVE_ME_FREE_FOOD");

	return 0;
}
