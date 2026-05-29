/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:30:00 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 14:55:12 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FragTrap.hpp"

//constructors
FragTrap::FragTrap() : ClapTrap() {
	_hitPoints = 100;
	_energyPoints = 100;
	_attackDamage = 30;

	std::cout << "FragTrap default constructor called" << std::endl;
}

FragTrap::FragTrap(const FragTrap &other) : ClapTrap(other) {
	std::cout << "FragTrap Copy ctor " << _name << " has been called!" << std::endl;
}

FragTrap::FragTrap(std::string name) : ClapTrap(name) {
	_hitPoints = 100;
	_energyPoints = 100;
	_attackDamage = 30;

	std::cout << "FragTrap ctor for " << _name << " has been called" << std::endl;
}

//overloaded operator
FragTrap &FragTrap::operator=(FragTrap const &other) {
	std::cout << "FragTrap assignation operator called" << std::endl;

	if (this != &other)
	{
		ClapTrap::operator=(other);
		std::cout << "FragTrap " << _name
					<< " has been assigned" << std::endl;
	}
	return (*this);
}

//destructor
FragTrap::~FragTrap() {
	std::cout << "FragTrap destructor for "
				<< _name << " has been called!" << std::endl;
}

//highFivesGuys
void FragTrap::highFivesGuys(void) {
	std::cout << "FragTrap " << _name
				<< " requests a positive high five!" << std::endl;
}
