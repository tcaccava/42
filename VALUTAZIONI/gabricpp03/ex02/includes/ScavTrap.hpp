/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:50:35 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 15:50:45 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAVTRAP_HPP
#define SCAVTRAP_HPP

#include "ClapTrap.hpp"

/*
** ScavTrap Class - First Derived Class from ClapTrap
**
** Demonstrates that multiple different classes can derive from the same base.
**
** INHERITANCE HIERARCHY IN EX02:
**      ClapTrap (base)
**       /    \
**  ScavTrap  FragTrap
**
** ScavTrap and FragTrap are "siblings" - both derive from ClapTrap,
** but are independent of each other.
** SCAVTRAP'S DESIGN CHOICES:
** - Different stats: 100 HP, 50 Energy, 20 Attack
** - OVERRIDES attack() with "fiercely attacks" message
** - INHERITS takeDamage() and beRepaired() unchanged
** - ADDS guardGate() special ability
**
** COMPARISON WITH FRAGTRAP:
** - ScavTrap: 100/50/20 stats, overrides attack(), has guardGate()
** - FragTrap:  100/100/30 stats, inherits attack(), has highFivesGuys()
*/
class	ScavTrap : public ClapTrap {
	private:

		bool	_guardingGate;

	public:
		ScavTrap();
		ScavTrap(std::string name);
		ScavTrap(const ScavTrap& other);
		ScavTrap	&operator=(ScavTrap const &other);
		virtual ~ScavTrap();

		void	attack(const std::string &target);
		void	guardGate();
};

#endif
