/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:54:21 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/10 12:11:38 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

int	main(void) {
	ClapTrap p1;
	ClapTrap p2("Alpha");
	ClapTrap p3(p2);
	ClapTrap p4 = p1;

	p1.setAttackDamage(1);
	p2.setAttackDamage(1);
	p3.setAttackDamage(1);
	p4.setAttackDamage(1);

	p1.attack(p2.getName());
	p2.takeDamage(p1.getAttackDamage());
	p2.beRepaired(3);

	p2.attack(p1.getName());
	p1.takeDamage(p2.getAttackDamage());

	p3.attack("Dummy");
	p4.beRepaired(5);

	while (p2.getHitPoints() > 0) {
		p1.attack(p2.getName());
		p2.takeDamage(p1.getAttackDamage());
		std::cout << p2.getName() << " has " << p2.getHitPoints() << " HP remaining" << std::endl;
	}

	p2.attack("Nobody");
	p2.beRepaired(2);

	return 0;
}
