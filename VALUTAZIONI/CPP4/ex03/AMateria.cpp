/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMateria.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 02:32:08 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 18:04:10 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include "AMateria.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AMateria::AMateria()
	: _type("default")
{
	std::cout << "AMateria default constructor called." << std::endl;
}

AMateria::AMateria(AMateria const &src)
	: _type(src._type)
{
	std::cout << "AMateria copy constructor called." << std::endl;
}

AMateria::AMateria(std::string const &type)
	: _type(type)
{
	std::cout << "AMateria type constructor called." << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AMateria::~AMateria()
{
	std::cout << "AMateria destructor called." << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AMateria &AMateria::operator=(AMateria const &rhs)
{
	if (this != &rhs)
	{
		this->_type = rhs._type;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string const &AMateria::getType() const
{
	return this->_type;
}

/* ************************************************************************** */
