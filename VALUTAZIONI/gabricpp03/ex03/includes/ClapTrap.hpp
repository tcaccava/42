/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:50:25 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 17:18:34 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLAPTRAP_HPP
#define CLAPTRAP_HPP

#include <iostream>

/*
** Base class for the inheritance hierarchy
**
** This is the foundation of our inheritance tree:
**     ClapTrap (base class)
**      /    \
**  ScavTrap  FragTrap (derived classes with virtual inheritance)
**      \    /
**   DiamondTrap (most derived class - combines both)
**
** DEFAULT ATTRIBUTES:
** - Name: passed as parameter to constructor
** - Hit points: 10
** - Energy points: 10
** - Attack damage: 0
**
** PROTECTED ATTRIBUTES:
** Members are protected (not private) to allow derived classes to access them
** This is necessary for ScavTrap, FragTrap, and DiamondTrap to modify these values
**
** VIRTUAL DESTRUCTOR:
** The destructor is virtual to ensure proper cleanup in inheritance hierarchies
** When deleting through a base class pointer, virtual ensures derived destructors are called
*/
class ClapTrap {
	protected:
		// Protected attributes - accessible by derived classes
		std::string			_name;
		unsigned int		_hitPoints;
		unsigned int		_energyPoints;
		unsigned int		_attackDamage;

	public:
		// (OCF)
		ClapTrap();
		ClapTrap(const std::string name);
		ClapTrap(const ClapTrap& other);
		ClapTrap	&operator=(const ClapTrap &other);
		virtual	~ClapTrap();						// Virtual destructor (IMPORTANT!)

		// Public methods 
		void			attack(const std::string &target);
		void					takeDamage(unsigned int amount);
		void					beRepaired(unsigned int amount);
};

#endif
