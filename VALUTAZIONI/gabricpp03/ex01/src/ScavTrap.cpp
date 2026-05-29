/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:50:40 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 15:36:14 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScavTrap.hpp"

/*
** INITIALIZATION LIST: : ClapTrap()
** First calls ClapTrap's default constructor to initialize inherited attributes.
** Then initializes ScavTrap-specific attributes in the constructor body.
**
** OUTPUT SEQUENCE:
** "Default constructor called"        <- from ClapTrap()
** "ScavTrap default constructor called" <- from ScavTrap()
*/
ScavTrap::ScavTrap() : ClapTrap() {
	_hitPoints = 100;
	_energyPoints = 50;
	_attackDamage = 20;
	_guardingGate = false;

	std::cout << "ScavTrap default constructor called" << std::endl;
}

/*
** Copy Constructor
** INITIALIZATION LIST: : ClapTrap(other)
** Calls ClapTrap's copy constructor to copy inherited attributes.
** The ClapTrap copy constructor handles _name, _hitPoints, _energyPoints, _attackDamage.
** We only need to manually copy ScavTrap-specific attributes (_guardingGate).
**
** Why this works:
** ClapTrap(other) creates a ClapTrap from the ClapTrap part of 'other' (slicing).
*/
ScavTrap::ScavTrap(const ScavTrap &other) : ClapTrap(other) {
	_guardingGate = other._guardingGate;
	std::cout << "ScavTrap Copy constructor " << _name << " has been called!" << std::endl;
}

/*
** INITIALIZATION LIST: : ClapTrap(name)
** Calls ClapTrap's parametric constructor to set the name.
** Then overrides the default stats with ScavTrap-specific values.
**
** EXECUTION ORDER:
** 1. ClapTrap(name) runs -> sets _name, _hitPoints=10, _energyPoints=10, _attackDamage=0
** 2. ScavTrap constructor body runs -> overrides HP, Energy, Attack to ScavTrap values
** 3. Prints ScavTrap message
**
** OUTPUT SEQUENCE:
** "ClapTrap Robot has been called"        <- from ClapTrap(name)
** "ScavTrap constructor for Robot has been called" <- from ScavTrap
*/
ScavTrap::ScavTrap(std::string name) : ClapTrap(name) {
	_hitPoints = 100;
	_energyPoints = 50;
	_attackDamage = 20;
	_guardingGate = false;

	std::cout << "ScavTrap constructor for " << _name << " has been called" << std::endl;
}

ScavTrap	&ScavTrap::operator=(ScavTrap const &other) {
	std::cout << "ScavTrap assignation operator called" << std::endl;

	if (this != &other)
	{
		ClapTrap::operator=(other);
		_guardingGate = other._guardingGate;
		std::cout << "ScavTrap " << _name
					<< " has been assigned" << std::endl;
	}
	return (*this);
}

ScavTrap::~ScavTrap() {
	std::cout << "ScavTrap destructor for " << _name << " has been called!" << std::endl;
}

/*
** Attack Method - OVERRIDDEN
** This REPLACES ClapTrap's attack() method.
** ScavTrap attacks are "fiercer" (different message) and deal 20 damage (not 0).
**
** METHOD OVERRIDING:
** When you call: scav.attack("target")
** This ScavTrap::attack() is called, NOT ClapTrap::attack()
*/
void	ScavTrap::attack(const std::string &target) {
	if (_energyPoints <= 0)
	{
		std::cout << "ScavTrap " << _name
					<< " has no energy to attack" << std::endl;
		return ;
	}
	if (_hitPoints <= 0)
	{
		std::cout << "ScavTrap " << _name
					<< " died and is not able to attack" << std::endl;
		return ;
	}
	std::cout << "ScavTrap " << _name << " fiercely attacks "
				<< target << ", causing " << _attackDamage
				<< " points of damage!" << std::endl;
	_energyPoints--;
}

void	ScavTrap::guardGate() {
	if (_guardingGate == false)
	{
		_guardingGate = true;
		std::cout << "ScavTrap " << _name
					<< " is now in Gate keeper mode!" << std::endl;
	}
	else
		std::cout << "ScavTrap " << this->_name
					<< " is already guarding the gate." << std::endl;
}
