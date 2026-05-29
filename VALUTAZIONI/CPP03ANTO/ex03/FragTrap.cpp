/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:19:38 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 09:38:56 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FragTrap.hpp"

FragTrap::FragTrap( void ) : ClapTrap() {
	std::cout << "FragTrap default constructor called" << std::endl;
	hitPoints = 100;
	energyPoints = 100;
	attackDamage = 30;
}

FragTrap::FragTrap( const std::string& newName ) : ClapTrap(newName) {
	std::cout << "FragTrap constructor with parameters called" << std::endl;
	hitPoints = 100;
	energyPoints = 100;
	attackDamage = 30;
}

FragTrap::FragTrap( const FragTrap& fragTrap ) : ClapTrap(fragTrap) {
	std::cout << "FragTrap copy constructor called" << std::endl;
}

FragTrap&	FragTrap::operator=(const FragTrap& fragTrap ) {
	std::cout << "FragTrap copy assignment constructor called" << std::endl;
	if (this != &fragTrap) {
		ClapTrap::operator=(fragTrap);
	}
	return (*this);
}

void	FragTrap::highFivesGuys( void ) {
	std::cout << "FragTrap " << name << " gave a high-five" << std::endl;
}

void	FragTrap::attack( const std::string& target ) {
	if (energyPoints > 0 && hitPoints > 0) {
		std::cout << "FragTrap " << name << " attacks " << target << ", causing " \
					<< attackDamage << " points of damage!";
		std::cout << std::endl;
		energyPoints -= 1;
	} else if (energyPoints == 0) {
		std::cout << "FragTrap " << name << " attack failed!\n";
		std::cout << "FragTrap " << name << " has no more energy points left.";
		std::cout << std::endl;
	}
	else {
		std::cout << "FragTrap " << name << " attack failed!\n";
		std::cout << "FragTrap " << name << " has no more hit points left.";
	}
	std::cout << std::endl;
}

FragTrap::~FragTrap( void ) {
	std::cout << "Fragtrap destructor called" << std::endl;
}
