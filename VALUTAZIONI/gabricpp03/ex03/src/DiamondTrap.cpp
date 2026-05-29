/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DiamondTrap.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:50:00 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 14:44:08 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DiamondTrap.hpp"

DiamondTrap::DiamondTrap() : ClapTrap("default_clap_name"), FragTrap(), ScavTrap()
{
	this->_name = "default";
	this->_hitPoints = FragTrap::_hitPoints;      // 100
	this->_energyPoints = ScavTrap::_energyPoints; // 50
	this->_attackDamage = FragTrap::_attackDamage; // 30
	
	std::cout << "DiamondTrap default constructor called" << std::endl;
}

/*
** Parametric constructor
** IMPORTANT: ClapTrap name becomes "<name>_clap_name"
** DiamondTrap's own _name stores the original name
** This allows us to distinguish between the two in whoAmI()
*/
DiamondTrap::DiamondTrap(std::string name) : ClapTrap(name + "_clap_name"), FragTrap(name), ScavTrap(name)
{
	this->_name = name;
	this->_hitPoints = FragTrap::_hitPoints;      // 100
	this->_energyPoints = ScavTrap::_energyPoints; // 50
	this->_attackDamage = FragTrap::_attackDamage; // 30

	std::cout << "DiamondTrap constructor for " << this->_name << " has been called" << std::endl;
}

/*
** Copy constructor
** Must call all parent copy constructors in the initialization list
** Then copy the DiamondTrap-specific _name attribute
*/
DiamondTrap::DiamondTrap(const DiamondTrap &other) : ClapTrap(other), FragTrap(other), ScavTrap(other)
{
	this->_name = other._name;
	std::cout << "DiamondTrap Copy constructor " << this->_name	<< " has been called!" << std::endl;
}

/*
** Assignment operator
** Must check for self-assignment (if (this != &other))
** Calls parent assignment operator to copy inherited attributes
** Then copies DiamondTrap-specific _name
*/
DiamondTrap &DiamondTrap::operator=(DiamondTrap const &other) {
	std::cout << "DiamondTrap assignation operator called" << std::endl;

	if (this != &other)
	{
		ClapTrap::operator=(other);
		this->_name = other._name;
		std::cout << "DiamondTrap " << this->_name
					<< " has been assigned" << std::endl;
	}
	return (*this);
}

/*
** Destructor
** Virtual destructors ensure proper cleanup in inheritance hierarchies
** Destruction order: DiamondTrap -> ScavTrap -> FragTrap -> ClapTrap
** This is the REVERSE of construction order
*/
DiamondTrap::~DiamondTrap() {
	std::cout << "DiamondTrap destructor for " 
			<< this->_name << " has been called!" << std::endl;
}

/*
** whoAmI() - Special method required by subject
** Demonstrates that DiamondTrap has TWO names:
** 1. this->_name: The DiamondTrap's own private name
** 2. ClapTrap::_name: The inherited name (with "_clap_name" suffix)
**
** We use scope resolution (ClapTrap::_name) to explicitly access
** the base class member, even though it's protected and we could
** access it directly. This makes the code clearer.
*/
void DiamondTrap::whoAmI() {
	std::cout << "=== Who Am I? ===" << std::endl;
	std::cout << "My DiamondTrap name is: " << this->_name << std::endl;
	std::cout << "My ClapTrap name is: " << ClapTrap::_name << std::endl;
	std::cout << "================" << std::endl;
}
 attack
 using Scavtrap::attack()