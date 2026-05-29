/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:32:21 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 11:39:36 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FragTrap.hpp"

int	main(void) {
	FragTrap defaultScav;
	FragTrap guardianScav("Guardian");
	FragTrap copyScav(guardianScav);
	FragTrap assignScav = defaultScav;

	defaultScav.highFivesGuys();
	guardianScav.highFivesGuys();
	copyScav.highFivesGuys();

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
	guardianScav.highFivesGuys();

	return 0;
}
