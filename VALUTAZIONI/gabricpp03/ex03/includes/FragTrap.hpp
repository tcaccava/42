/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:30:00 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 16:51:09 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRAGTRAP_HPP
#define FRAGTRAP_HPP

#include "ClapTrap.hpp"

/*
** FragTrap class - Derived from ClapTrap
**
** VIRTUAL INHERITANCE: "virtual public ClapTrap"
** - Same as ScavTrap, uses virtual inheritance
** - This allows both FragTrap and ScavTrap to be combined in DiamondTrap
** - Without virtual inheritance, the diamond problem would occur
**
** ATTRIBUTES (different from ClapTrap):
** - Hit points: 100 (instead of 10)
** - Energy points: 100 (instead of 10)
** - Attack damage: 30 (instead of 0)
**
** SPECIAL ABILITY:
** - highFivesGuys(): Requests a positive high five
**
** NOTE: attack() method declared but not used in DiamondTrap
** DiamondTrap uses ScavTrap::attack() instead
*/
class FragTrap : virtual public ClapTrap {
	public:
		// (OCF)
		FragTrap();
		FragTrap(const std::string name);
		FragTrap(const FragTrap& other);
		FragTrap	&operator=(FragTrap const &other);
		virtual ~FragTrap();

		// Special ability unique to FragTrap
		void	highFivesGuys(void);

};

#endif
