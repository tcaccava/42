/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:53 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 19:18:32 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dog.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Dog::Dog()
	: type("Dog"), brain(new Brain())
{
	std::cout << "Dog default constructor called." << std::endl;
}

Dog::Dog(const Dog &src)
	: Animal(src), type(src.type), brain(new Brain(*src.brain))
{
	std::cout << "Dog copy constructor called." << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Dog::~Dog()
{
	std::cout << "Dog destructor called." << std::endl;
	delete this->brain;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Dog &Dog::operator=(Dog const &rhs)
{
	std::cout << "Dog assingment operator" << std::endl;
	if (this != &rhs)
	{
		if (this->brain)
			delete this->brain;
		this->brain = new Brain(*rhs.brain);
		this->type = rhs.type;
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

std::string Dog::getIdea(int i)
{
	if (!this->brain)
		return "No brain";
	return this->brain->getIdea(i);
}

void Dog::setIdea(int i, std::string idea)
{
	if (this->brain)
		this->brain->setIdea(i, idea);
}

std::string const &Dog::getType(void) const
{
	return (this->type);
}

void Dog::setType(std::string const &type)
{
	this->type = type;
}

/* ************************************************************************** */
