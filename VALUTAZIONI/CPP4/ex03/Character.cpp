/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Character.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:08:03 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 18:11:38 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include "AMateria.hpp"
#include "Character.hpp"
#include "ICharacter.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Character::Character()
	: _name("Default Character"), _inventory()
{
	std::cout << "Character default constructor called." << std::endl;
}

Character::Character(const std::string name)
	: _name(name), _inventory()
{
	std::cout << "Character name constructor called." << std::endl;
}

Character::Character(const Character &src)
	: _name(src._name), _inventory()
{
	std::cout << "Character copy constructor called." << std::endl;
	int i = 0;
	while (i < 4)
	{
		if (src._inventory[i])
			this->_inventory[i] = src._inventory[i]->clone();
		else
			this->_inventory[i] = 0;
		i++;
	}
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Character::~Character()
{
	std::cout << "Character destructor called." << std::endl;
	int i = 0;
	while (i < 4)
	{
		if (this->_inventory[i])
			delete this->_inventory[i];
		i++;
	}
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Character &Character::operator=(Character const &rhs)
{
	if (this != &rhs)
	{
		this->_name = rhs._name;
		int i = 0;
		while (i < 4)
		{
			if (this->_inventory[i])
				delete (this->_inventory[i]);
			this->_inventory[i] = 0;
			if (rhs._inventory[i])
				this->_inventory[i] = rhs._inventory[i]->clone();
			i++;
		}
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

std::string const &Character::getName() const
{
	return this->_name;
}

void Character::equip(AMateria *m)
{
	int i = 0;
	while (i < 4 && this->_inventory[i])
		i++;
	if (i < 4)
		this->_inventory[i] = m;
}

void Character::unequip(int idx)
{
	if (idx >= 0 && idx < 4)
		this->_inventory[idx] = 0;
}
void Character::use(int idx, ICharacter &target)
{
	if (idx >= 0 && idx < 4 && this->_inventory[idx])
		this->_inventory[idx]->use(target);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */
