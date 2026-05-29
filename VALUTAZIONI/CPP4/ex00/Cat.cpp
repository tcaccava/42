/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:22 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 18:55:29 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cat::Cat()
	: type("Cat")
{
	std::cout << "Cat default constructor called." << std::endl;
}

Cat::Cat(const Cat &src)
	: Animal(src), type(src.type)
{
	std::cout << "Cat copy constructor called." << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Cat::~Cat()
{
	std::cout << "Cat destructor called." << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Cat &Cat::operator=(Cat const &rhs)
{
	std::cout << "Cat assingment operator" << std::endl;
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

void Cat::makeSound(void) const
{
	std::cout << "Meow!" << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string const &Cat::getType(void) const
{
	return (this->type);
}

void Cat::setType(std::string const &type)
{
	this->type = type;
}

/* ************************************************************************** */
