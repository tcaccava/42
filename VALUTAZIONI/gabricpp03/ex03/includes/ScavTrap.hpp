/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:50:35 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 17:18:49 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAVTRAP_HPP
#define SCAVTRAP_HPP

#include "ClapTrap.hpp"

/*
** ScavTrap class - Derived from ClapTrap
**
** VIRTUAL INHERITANCE: "virtual public ClapTrap"
** - The "virtual" keyword is CRUCIAL for solving the diamond problem
** - Without it, DiamondTrap would have two separate ClapTrap subobjects
** - Virtual inheritance ensures only ONE ClapTrap base in the final hierarchy
**
** ATTRIBUTES (different from ClapTrap):
** - Hit points: 100 (instead of 10)
** - Energy points: 50 (instead of 10)
** - Attack damage: 20 (instead of 0)
**
** SPECIAL ABILITY:
** - guardGate(): Puts ScavTrap in Gate keeper mode
*/
class ScavTrap : virtual public ClapTrap {
	private:
		// ScavTrap-specific attribute
		// Tracks whether this ScavTrap is currently guarding a gate
		bool	_guardingGate;

	public:
		// /(OCF)
		ScavTrap();
		ScavTrap(std::string name);
		ScavTrap(const ScavTrap& other);
		ScavTrap	&operator=(ScavTrap const &other);
		virtual ~ScavTrap();

		// Overridden method - ScavTrap attacks differently than ClapTrap
		// This will be used by DiamondTrap (via "using ScavTrap::attack")
		void	attack(const std::string &target);

		// Special ability unique to ScavTrap
		void	guardGate();
};

#endif
