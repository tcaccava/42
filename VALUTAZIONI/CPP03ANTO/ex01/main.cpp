/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:54:21 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/10 16:18:15 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScavTrap.hpp"

int	main(void) {
	ScavTrap defaultScav;
	ScavTrap guardianScav("Guardian");
	ScavTrap copyScav(guardianScav);
	ScavTrap assignScav = defaultScav;

	defaultScav.guardGate();
	guardianScav.guardGate();
	copyScav.guardGate();

	defaultScav.attack(guardianScav.getName());
	guardianScav.takeDamage(defaultScav.getAttackDamage());
	guardianScav.beRepaired(5);

	guardianScav.attack(defaultScav.getName());
	defaultScav.takeDamage(guardianScav.getAttackDamage());

	copyScav.attack("Enemy");
	assignScav.beRepaired(3);

	while (guardianScav.getEnergyPoints() > 0) {
		guardianScav.attack("Target");
	}

	guardianScav.attack("Target");
	guardianScav.beRepaired(1);
	guardianScav.guardGate();

	return 0;
}
