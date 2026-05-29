/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:50:35 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 15:32:31 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAVTRAP_HPP
#define SCAVTRAP_HPP

#include "ClapTrap.hpp"

/*
** ScavTrap Class - Derived from ClapTrap
**
** Demonstrates PUBLIC INHERITANCE in C++.
** Syntax: class Derived : public Base
**
** INHERITANCE RELATIONSHIP:
** ScavTrap "is-a" ClapTrap (inherits all ClapTrap's attributes and methods)
** 
** PUBLIC INHERITANCE means:
** - Public members of ClapTrap remain public in ScavTrap
** - Protected members of ClapTrap remain protected in ScavTrap
** - Private members of ClapTrap are NOT accessible (but still exist)
**
** DIFFERENCES FROM CLAPTRAP:
** - Different stats: 100 HP, 50 Energy, 20 Attack (vs 10/10/0)
** - Different attack message
** - Additional feature: guardGate()
*/
class	ScavTrap : public ClapTrap {
	private:

		bool	_guardingGate;

	public:
		ScavTrap();
		ScavTrap(std::string name);
		ScavTrap(const ScavTrap& other);
		ScavTrap	&operator=(ScavTrap const &other);
		~ScavTrap();

		/*
		** attack() - OVERRIDDEN METHOD
		** This replaces ClapTrap's attack() with ScavTrap's own version.
		** ScavTrap attacks have different messages than ClapTrap attacks.
		** This is called METHOD OVERRIDING
		*/
		void	attack(const std::string &target);
		/*
		** guardGate() - NEW METHOD
		** Special ability unique to ScavTrap (not in ClapTrap).
		** Puts the ScavTrap into "Gate keeper mode".
		*/
		void	guardGate();
};

#endif
