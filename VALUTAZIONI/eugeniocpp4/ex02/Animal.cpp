/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonacco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 21:37:53 by ebonacco          #+#    #+#             */
/*   Updated: 2026/03/26 21:37:55 by ebonacco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "Animal.hpp"

Animal::Animal() : type("Animal")
{
	std::cout << "Animal Default constructor" << std::endl;
}

Animal::Animal(std::string type) : type(type)
{
	std::cout << "Animal Parametric constructor" << std::endl;
}

Animal::Animal(const Animal &obj) : type(obj.type)
{
	std::cout << "Animal Copy constructor" << std::endl;
}

Animal& Animal::operator=(const Animal &obj)
{
	if (this != &obj)
		type = obj.type;
	std::cout << "Animal Copy operator constructor" << std::endl;
	return *this;
}

Animal::~Animal()
{
	std::cout << "Animal Destructor" << std::endl;
}

void Animal::makeSound() const
{
	std::cout << "Some generic animal sound" << std::endl;
}

void Animal::setType(const std::string &type)
{
	this->type = type;
}

const std::string &Animal::getType() const
{
	return this->type;
}
