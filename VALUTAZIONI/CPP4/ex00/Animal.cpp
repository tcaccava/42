/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:15 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 18:53:16 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Animal::Animal()
	: type("Unkown")
{
	std::cout << "Animal default constructor called." << std::endl;
}

Animal::Animal(const Animal &src)
	: type(src.type)
{
	std::cout << "Animal copy constructor called." << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Animal::~Animal()
{
	std::cout << "Animal destructor called." << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Animal &Animal::operator=(Animal const &rhs)
{
	std::cout << "Animal assingment operator" << std::endl;
	if (this != &rhs)
		this->type = rhs.type;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void Animal::makeSound(void) const
{
	std::cout << "Ring-ding-ding-ding-dingeringeding!" << std::endl;
	std::cout << "Gering-ding-ding-ding-dingeringeding!" << std::endl;
	std::cout << "Gering-ding-ding-ding-dingeringeding!" << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const std::string Animal::getType(void) const
{
	return (this->type);
}

void Animal::setType(std::string type)
{
	this->type = type;
}

/* ************************************************************************** */
