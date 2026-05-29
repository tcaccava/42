/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:49:15 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/10 16:30:48 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScavTrap.hpp"

ScavTrap::ScavTrap( void ) : ClapTrap::ClapTrap() {
	hitPoints = 100;
	energyPoints = 50;
	attackDamage = 20;
	std::cout << "Scavtrap default constructor called" << std::endl;
}

ScavTrap::ScavTrap( const std::string& newName ) : ClapTrap::ClapTrap(newName) {
	hitPoints = 100;
	energyPoints = 50;
	attackDamage = 20;
	std::cout << "Scavtrap constructor with parameters called" << std::endl;
}

ScavTrap::ScavTrap( const ScavTrap& scavTrap ) : ClapTrap::ClapTrap(scavTrap) {
	hitPoints = 100;
	energyPoints = 50;
	attackDamage = 20;
	std::cout << "Scavtrap copy constructor called" << std::endl;
}

ScavTrap&	ScavTrap::operator=(const ScavTrap& scavTrap ) {
	std::cout << "Scavtrap copy assignment constructor called" << std::endl;
	if (this != &scavTrap) {
		ClapTrap::operator=(scavTrap);
	}
	return (*this);
}

void	ScavTrap::guardGate( void ) {
	std::cout << "Scavtrap is now in guard-gate mode!" << std::endl;
}

void	ScavTrap::attack( const std::string& target ) {
	if (energyPoints > 0 && hitPoints > 0) {
		std::cout << "Scavtrap " << name << " attacks " << target << ", causing " \
					<< attackDamage << " points of abnormous damage!";
		std::cout << std::endl;
		energyPoints -= 1;
	} else if (energyPoints == 0) {
		std::cout << "Scavtrap " << name << " attack failed!\n";
		std::cout << "Scavtrap " << name << " has no more energy points left.";
		std::cout << std::endl;
	}
	else {
		std::cout << "Scavtrap " << name << " attack failed!\n";
		std::cout << "Scavtrap " << name << " has no more hit points left.";
	}
	std::cout << std::endl;
}

ScavTrap::~ScavTrap( void ) {
	std::cout << "Scavtrap destructor called" << std::endl;
}


