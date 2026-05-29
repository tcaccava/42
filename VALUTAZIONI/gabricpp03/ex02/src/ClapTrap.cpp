/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:50:21 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 15:51:21 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

ClapTrap::ClapTrap() {
	std::cout << "Default constructor called" << std::endl;
}

ClapTrap::ClapTrap(std::string name)
		: _name(name), _hitPoints(10), _energyPoints(10), _attackDamage(0) {
	std::cout << "ClapTrap " << _name << " has been called"
				<< std::endl;
}

ClapTrap::ClapTrap(const ClapTrap &other)
			: _name(other._name),
				_hitPoints(other._hitPoints),
				_energyPoints(other._energyPoints),
				_attackDamage(other._attackDamage) {

	std::cout << "ClapTrap " << _name << " has been copied!"
				<< std::endl;
}

ClapTrap &ClapTrap::operator=(ClapTrap const &other) {
	if (this != &other)
	{
		_name = other._name;
		_hitPoints = other._hitPoints;
		_energyPoints = other._energyPoints;
		_attackDamage = other._attackDamage;
		std::cout << "ClapTrap " << _name << " has been assigned"
					<< std::endl;
	}
	return (*this);
}

ClapTrap::~ClapTrap(){
	std::cout << "ClapTrap " << _name << " has been destroyed!"
				<< std::endl;
}

void ClapTrap::attack(const std::string &target){
	if (_energyPoints <= 0)
	{
		std::cout << "ClapTrap " << _name << " has no energy to attack"
					<< std::endl;
		return ;
	}
	if (_hitPoints <= 0)
	{
		std::cout << "ClapTrap " << _name
					<< " died and is not able to attack" << std::endl;
		return ;
	}
	_energyPoints--;
	std::cout << "ClapTrap " << _name << " attacks " << target << ", causing "
			<< _attackDamage << " points of damage" << std::endl;
}

void	ClapTrap::takeDamage(unsigned int amount){
	std::cout << "ClapTrap " << _name << " takes " << amount
				<< " points of damage" << std::endl;
	if (amount >= _hitPoints)
	{
		_hitPoints = 0;
		std::cout << "ClapTrap " << _name << " is dead" << std::endl;
	}
	else
	{
		_hitPoints -= amount;
		std::cout << "ClapTrap " << _name << " has now "
					<< _hitPoints << " hit Points" << std::endl;
	}
}

void	ClapTrap::beRepaired(unsigned int amount) {
	if (_energyPoints <= 0)
	{
		std::cout << "ClapTrap " << _name
					<< " has no energy to repair itself" << std::endl;
		return ;
	}
	if (_hitPoints <= 0)
	{
		std::cout << "ClapTrap " << _name
					<< " died and is not able to repair itself" << std::endl;
		return ;
	}
	_hitPoints += amount;
	_energyPoints--;
	std::cout << "ClapTrap " << _name
				<< " is repairing itself with " << amount << " hit points" << std::endl;
	std::cout << "ClapTrap " << _name
				<< " has now " << _hitPoints << " hit points" << std::endl;
}
