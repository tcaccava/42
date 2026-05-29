/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:50:40 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 14:14:30 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScavTrap.hpp"

//constructors
ScavTrap::ScavTrap() : ClapTrap() {
	_hitPoints = 100;
	_energyPoints = 50;
	_attackDamage = 20;
	_guardingGate = false;

	std::cout << "ScavTrap default constructor called" << std::endl;
}

ScavTrap::ScavTrap(const ScavTrap &other) : ClapTrap(other) {
	_guardingGate = other._guardingGate;
	std::cout << "ScavTrap Copy ctor " << _name << " has been called!" << std::endl;
}

ScavTrap::ScavTrap(std::string name) : ClapTrap(name) {
	_hitPoints = 100;
	_energyPoints = 50;
	_attackDamage = 20;
	_guardingGate = false;

	std::cout << "ScavTrap ctor for " << _name << " has been called" << std::endl;
}


//overloaded operator
ScavTrap &ScavTrap::operator=(ScavTrap const &other) {
	std::cout << "ScavTrap assignation operator called" << std::endl;

	if (this != &other)
	{
		ClapTrap::operator=(other);
		_guardingGate = other._guardingGate;
		std::cout << "ScavTrap " << _name << " has been assigned" << std::endl;
	}
	return (*this);
}

//destructor
ScavTrap::~ScavTrap() {
	std::cout << "ScavTrap destructor for "
				<< _name << " has been called!" << std::endl;
}

//attack
void ScavTrap::attack(const std::string &target) {
	if (_energyPoints <= 0)
	{
		std::cout << "ScavTrap " << _name << " has no energy to attack" << std::endl;
		return ;
	}
	if (_hitPoints <= 0)
	{
		std::cout << "ScavTrap " << _name << " died and is not able to attack" << std::endl;
		return ;
	}
	std::cout << "ScavTrap " << _name << " fiercely attacks "
				<< target << ", causing " << _attackDamage
				<< " points of damage!" << std::endl;
	_energyPoints--;
}

void ScavTrap::guardGate() {
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
