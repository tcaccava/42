/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DiamondTrap.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:50:00 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 17:30:33 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIAMONDTRAP_HPP
#define DIAMONDTRAP_HPP

#include "ScavTrap.hpp"
#include "FragTrap.hpp"

/*
** Demonstrates the Diamond Problem and its solution
**
** THE DIAMOND PROBLEM:
**     ClapTrap
**      /    \
**  ScavTrap  FragTrap
**      \    /
**   DiamondTrap
**
** Without virtual inheritance, DiamondTrap would have TWO copies of ClapTrap,
** causing ambiguity.

** Virtual inheritance ensures only ONE ClapTrap base object exists.
**
** INHERITANCE HIERARCHY:
** - DiamondTrap inherits from BOTH ScavTrap and FragTrap
** - Both ScavTrap and FragTrap use VIRTUAL inheritance from ClapTrap
** - Result: Only one ClapTrap subobject in DiamondTrap
**
** ATTRIBUTE INHERITANCE (as per subject requirements):
** - Hit Points: 100 (from FragTrap)
** - Energy Points: 50 (from ScavTrap)
** - Attack Damage: 30 (from FragTrap)
** - Attack method: from ScavTrap (using declaration)
*/
class DiamondTrap : public FragTrap, public ScavTrap {
	private:
		// DiamondTrap has its own private name
		// This is DIFFERENT from ClapTrap::_name
		// ClapTrap::_name will be "<name>_clap_name"
		std::string	_name;

	public:
		//(OCF)
		DiamondTrap();
		DiamondTrap(const std::string name);
		DiamondTrap(const DiamondTrap& other);
		DiamondTrap	&operator=(DiamondTrap const &other);
		~DiamondTrap();

		// Special method to display both DiamondTrap name and ClapTrap name
		// This demonstrates that we have access to both the private _name
		// and the inherited ClapTrap::_name
		void	whoAmI();
};

#endif
