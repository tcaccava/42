/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:29:41 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 09:38:56 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

ClapTrap::ClapTrap( ) : name("(null)"), hitPoints(HIT_POINTS), energyPoints(ENERGY_POINTS), \
						attackDamage(ATTACK_DAMAGE) {
	std::cout << "ClapTrap default constructor called" << std::endl;
}

ClapTrap::ClapTrap( const std::string& newName ) : name(newName == "" ? "(null)" : newName), \
					hitPoints(HIT_POINTS), energyPoints(ENERGY_POINTS), attackDamage(ATTACK_DAMAGE) {
	std::cout << "Claptrap constructor with parameters called" << std::endl;
}

ClapTrap::ClapTrap( const ClapTrap& clapTrap ) {
	std::cout << "ClapTrap copy constructor called" << std::endl;
	name = clapTrap.name;
	hitPoints = clapTrap.hitPoints;
	energyPoints = clapTrap.energyPoints;
	attackDamage = clapTrap.attackDamage;
}

ClapTrap&	ClapTrap::operator=( const ClapTrap& clapTrap ) {
	std::cout << "ClapTrap copy assignment constructor called" << std::endl;
	if (this != &clapTrap) {
		name = clapTrap.name;
		hitPoints = clapTrap.hitPoints;
		energyPoints = clapTrap.energyPoints;
		attackDamage = clapTrap.attackDamage;
	}
	return (*this);
}

void	ClapTrap::attack( const std::string& target ) {

	if (energyPoints > 0 && hitPoints > 0) {
		std::cout << "ClapTrap " << name << " attacks " << target << ", causing " \
					<< attackDamage << " points of damage!";
		std::cout << std::endl;
		energyPoints -= 1;
	} else if (energyPoints == 0) {
		std::cout << "ClapTrap " << name << " attack failed!\n";
		std::cout << "ClapTrap " << name << " has no more energy points left.";
		std::cout << std::endl;
	}
	else {
		std::cout << "ClapTrap " << name << " attack failed!\n";
		std::cout << "ClapTrap " << name << " has no more hit points left.";
	}
	std::cout << std::endl;
}

void	ClapTrap::takeDamage( unsigned int amount ) {
	if (hitPoints > 0) {
		if (amount > hitPoints) {
			amount = hitPoints;
		}

		std::cout << "ClapTrap " << name << " has taken " << amount << " damage!";

		hitPoints -= amount;
	} else {
		std::cout << "Claptrap " << name << " cannot take damage anymore.\n";
		std::cout << "ClapTrap " << name << " has no more hit points left.";
	}
	std::cout << std::endl;
}

void	ClapTrap::beRepaired( unsigned int amount ) {
	if (energyPoints > 0 && hitPoints > 0) {
		std::cout << "ClapTrap " << name << " has regained " << amount \
					<< " hit points!";
		hitPoints += amount;
		energyPoints -= 1;
	} else if (energyPoints == 0) {
		std::cout << "ClapTrap " << name << " cannot repair itself anymore!\n";
		std::cout << "ClapTrap " << name << " is out of energy points.";
	} else {
		std::cout << "ClapTrap " << name << " cannot repair itself anymore!\n";
		std::cout << "ClapTrap " << name << " has no more hit points left.";
	}
	std::cout << std::endl;
}

ClapTrap::~ClapTrap( ) {
	std::cout << "ClapTrap destructor called" << std::endl;
}

const std::string&	ClapTrap::getName( void ) const {
	return (name);
}

const unsigned int&	ClapTrap::getHitPoints( void ) const {
	return (hitPoints);
}

const unsigned int&	ClapTrap::getEnergyPoints( void ) const {
	return (energyPoints);
}

const unsigned int&	ClapTrap::getAttackDamage( void ) const {
	return (attackDamage);
}

void	ClapTrap::setName( const std::string& newName ) {
	name = newName;
}

void	ClapTrap::setHitPoints( const unsigned int& newHitPoints ) {
	hitPoints = newHitPoints;
}

void	ClapTrap::setEnergyPoints( const unsigned int& newEnergyPoints ) {
	energyPoints = newEnergyPoints;
}

void	ClapTrap::setAttackDamage( const unsigned int& newAttackDamage ) {
	attackDamage = newAttackDamage;
}
