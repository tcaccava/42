/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:50:21 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 16:13:51 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

/*
** Default Constructor
** Creates a ClapTrap with uninitialized values.
** Used when no name is provided: ClapTrap robot;
*/
ClapTrap::ClapTrap() {
	std::cout << "Default constructor called" << std::endl;
}

/*
** Parametric Constructor
** Creates a ClapTrap with specified name and default stats.
** Uses member initializer list (more efficient than assignment).
** Example: ClapTrap robot("Robot1");
*/
ClapTrap::ClapTrap(std::string name)
			: _name(name), _hitPoints(10), _energyPoints(10), _attackDamage(0)
{
	std::cout << "ClapTrap " << _name << " has been called" << std::endl;
}

/*
** Copy Constructor
** Creates a new object as a copy of an existing one.
** Called when: ClapTrap robot2(robot1);
** Each attribute is copied individually using member initializer list.
*/
ClapTrap::ClapTrap(const ClapTrap &other)
				: _name(other._name),
					_hitPoints(other._hitPoints),
					_energyPoints(other._energyPoints),
					_attackDamage(other._attackDamage)
{
	std::cout << "ClapTrap " << _name << " has been copied!" << std::endl;
}

/*
** Assignment Operator
** Copies values from one existing object to another.
** Called when: robot2 = robot1;
** Must check for self-assignment to prevent issues.
** Must return *this to allow chaining: a = b = c;
*/
ClapTrap	&ClapTrap::operator=(ClapTrap const &other) {
	if (this != &other)
	{
		_name = other._name;
		_hitPoints = other._hitPoints;
		_energyPoints = other._energyPoints;
		_attackDamage = other._attackDamage;
		std::cout << "ClapTrap " << _name << " has been assigned" << std::endl;
	}
	return (*this);
}

/*
** Destructor
** Called automatically when object goes out of scope or is deleted.
** Cleans up resources (in this case, just prints a message).
** In derived classes, destructors are called in reverse order.
*/
ClapTrap::~ClapTrap() {
	std::cout << "ClapTrap " << _name << " has been destroyed!" << std::endl;
}

/*
** Attack Function
** Performs an attack action on a target.
** Costs 1 energy point per attack.
** Cannot attack if dead (HP <= 0) or out of energy.
*/
void	ClapTrap::attack(const std::string &target) {
	if (_energyPoints <= 0)
	{
		std::cout << "ClapTrap " << _name << " has no energy to attack" << std::endl;
		return ;
	}
	if (_hitPoints <= 0)
	{
		std::cout << "ClapTrap " << _name << " died and is not able to attack" << std::endl;
		return ;
	}
	_energyPoints--;
	std::cout << "ClapTrap " << _name << " attacks " << target << ", causing "
				<< _attackDamage << " points of damage" << std::endl;
}

/*
** Take Damage Function
** Reduces hit points when taking damage.
** Prevents unsigned integer underflow by checking if damage >= HP.
** If damage exceeds HP, set HP to 0 (unit dies).
*/
void	ClapTrap::takeDamage(unsigned int amount) {
	std::cout << "ClapTrap " << _name << " takes "
				<< amount << " points of damage" << std::endl;
	if (amount >= _hitPoints)
	{
		_hitPoints = 0;
		std::cout << "ClapTrap " << _name << " is dead" << std::endl;
	}
	else
	{
		_hitPoints -= amount;
		std::cout << "ClapTrap " << _name << " has now " << _hitPoints << " hit Points" << std::endl;
	}
}

/*
** Repair Function
** Restores hit points at the cost of 1 energy point.
** Cannot repair if dead or out of energy.
** No maximum HP limit enforced (HP can exceed initial value).
*/
void	ClapTrap::beRepaired(unsigned int amount) {
	if (_energyPoints <= 0)
	{
		std::cout << "ClapTrap " << _name << " has no energy to repair itself" << std::endl;
		return ;
	}
	if (_hitPoints <= 0)
	{
		std::cout << "ClapTrap " << _name << " died and is not able to repair itself" << std::endl;
		return ;
	}
	_hitPoints += amount;
	_energyPoints--;
	std::cout << "ClapTrap " << _name << " is repairing itself with " << amount << " hit points" << std::endl;
	std::cout << "ClapTrap " << _name << " has now " << _hitPoints << " hit points" << std::endl;
}

int ClapTrap::getHit() const {
	return _hitPoints;
}