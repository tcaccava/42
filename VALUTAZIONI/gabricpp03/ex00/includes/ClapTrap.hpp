/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:50:32 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 17:34:32 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLAPTRAP_HPP
#define CLAPTRAP_HPP

#include <iostream>

/*
** ClapTrap Class
** 
** A simple robot class that demonstrates:
** - Encapsulation (private attributes, public methods)
** - Orthodox Canonical Form (OCF) - the 4 required functions
** - Resource management (hit points, energy points)
** - Basic game mechanics (attack, take damage, repair)
**
** Orthodox Canonical Form (OCF) includes:
** 1. Default constructor
** 2. Copy constructor
** 3. Assignment operator
** 4. Destructor
*/
class	ClapTrap {
	private:

		std::string			_name;
		unsigned int		_hitPoints;
		unsigned int		_energyPoints;
		unsigned int		_attackDamage;

	public:
		ClapTrap();
		ClapTrap(std::string name);
		ClapTrap(const ClapTrap& other);
		ClapTrap	&operator=(ClapTrap const &other);
		~ClapTrap();

		void	attack(const std::string &target);
		void	takeDamage(unsigned int amount);
		void	beRepaired(unsigned int amount);
		int getHit() const;
};

#endif
