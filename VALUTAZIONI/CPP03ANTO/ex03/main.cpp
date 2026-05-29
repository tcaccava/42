/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:55:10 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 14:04:21 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DiamondTrap.hpp"

int	main(void) {
	DiamondTrap defaultScav;
	DiamondTrap guardianScav("Guardian");
	DiamondTrap copyScav(guardianScav);
	DiamondTrap assignScav = defaultScav;

	defaultScav.guardGate();
	guardianScav.guardGate();
	copyScav.guardGate();

	defaultScav.whoAmI();
	defaultScav.attack(guardianScav.getName());
	guardianScav.takeDamage(defaultScav.getAttackDamage());
	guardianScav.beRepaired(5);

	guardianScav.whoAmI();
	guardianScav.attack(defaultScav.getName());
	defaultScav.takeDamage(guardianScav.getAttackDamage());

	copyScav.whoAmI();
	copyScav.attack("Enemy");
	assignScav.beRepaired(3);

	guardianScav.whoAmI();
	while (guardianScav.getEnergyPoints() > 0) {
		guardianScav.attack("Target");
	}

	guardianScav.attack("Target");
	guardianScav.beRepaired(1);
	guardianScav.guardGate();

	return 0;
}
