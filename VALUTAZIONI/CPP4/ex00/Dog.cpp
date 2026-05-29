/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:53 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 18:55:47 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dog.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Dog::Dog()
{
	this->type = "Dog";
	std::cout << "Dog default constructor called." << std::endl;
}

Dog::Dog(const Dog &src)
	: Animal(src)
{
	std::cout << "Dog copy constructor called." << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Dog::~Dog()
{
	std::cout << "Dog destructor called." << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Dog &Dog::operator=(Dog const &rhs)
{
	std::cout << "Dog assingment operator" << std::endl;
	if (this != &rhs)
	{
		this->type = rhs.type;
		this->Animal::type = rhs.Animal::type;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void Dog::makeSound(void) const
{
	std::cout << "Wuff!" << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string const &Dog::getType(void) const
{
	return (this->type);
}

void Dog::setType(std::string const &type)
{
	this->type = type;
}

/* ************************************************************************** */
