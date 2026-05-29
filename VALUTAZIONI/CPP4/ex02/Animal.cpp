/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:15 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 19:47:53 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AAnimal::AAnimal()
	: type("Unkown")
{
	std::cout << "Animal default constructor called." << std::endl;
}

AAnimal::AAnimal(const AAnimal &src)
	: type(src.type)
{
	std::cout << "Animal copy constructor called." << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AAnimal::~AAnimal()
{
	std::cout << "Animal destructor called." << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AAnimal &AAnimal::operator=(AAnimal const &rhs)
{
	std::cout << "Animal assingment operator" << std::endl;
	if (this != &rhs)
		this->type = rhs.type;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const std::string AAnimal::getType(void) const
{
	return (this->type);
}

void AAnimal::setType(std::string const &type)
{
	this->type = type;
}

/* ************************************************************************** */
