/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:30:00 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 15:54:15 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FragTrap.hpp"

/*
** Default Constructor
** INITIALIZATION LIST: : ClapTrap()
** Calls ClapTrap's default constructor, then overrides stats.
**
** EXECUTION ORDER:
** 1. ClapTrap() runs
**    - Prints: "Default constructor called"
** 2. FragTrap body runs
**    - Sets FragTrap-specific stats: 100 HP, 100 Energy, 30 Attack
**    - Prints: "FragTrap default constructor called"
*/
FragTrap::FragTrap() : ClapTrap() {
	_hitPoints = 100;
	_energyPoints = 100;
	_attackDamage = 30;

	std::cout << "FragTrap default constructor called"
				<< std::endl;
}

/*
** Copy Constructor
** INITIALIZATION LIST: : ClapTrap(other)
** Calls ClapTrap's copy constructor to copy all inherited attributes.
**
** FragTrap has NO unique private attributes (unlike ScavTrap's _guardingGate).
** Therefore, ClapTrap(other) copies everything we need:
** - _name
** - _hitPoints (already 100 from the original FragTrap)
** - _energyPoints (already 100 from the original FragTrap)
** - _attackDamage (already 30 from the original FragTrap)
**
*/
FragTrap::FragTrap(const FragTrap &other) : ClapTrap(other) {
	std::cout << "FragTrap Copy ctor " << _name
				<< " has been called!" << std::endl;
}

/*
** Parametric Constructor
** INITIALIZATION LIST: : ClapTrap(name)
** Calls ClapTrap's parametric constructor to set the name.
**
** EXECUTION ORDER:
** 1. ClapTrap(name) runs
**    - Sets _name = name, _hitPoints = 10, _energyPoints = 10, _attackDamage = 0
**    - Prints: "ClapTrap [name] has been called"
** 2. FragTrap body runs
**    - Overrides stats to FragTrap values: 100/100/30
**    - Prints: "FragTrap ctor for [name] has been called"
*/
FragTrap::FragTrap(std::string name) : ClapTrap(name) {
	_hitPoints = 100;
	_energyPoints = 100;
	_attackDamage = 30;

	std::cout << "FragTrap ctor for " << _name
				<< " has been called" << std::endl;
}

FragTrap &FragTrap::operator=(FragTrap const &other) {
	std::cout << "FragTrap assignation operator called" << std::endl;

	if (this != &other)
	{
		// Copy all inherited attributes
		ClapTrap::operator=(other);
		std::cout << "FragTrap " << _name
					<< " has been assigned" << std::endl;
	}
	return (*this);
}

FragTrap::~FragTrap() {
	std::cout << "FragTrap destructor for "
				<< _name << " has been called!" << std::endl;
}

void FragTrap::highFivesGuys(void) {
	std::cout << "FragTrap " << _name
				<< " requests a positive high five!" << std::endl;
}
