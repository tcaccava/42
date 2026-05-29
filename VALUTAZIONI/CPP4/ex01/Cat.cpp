/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:22 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 19:42:01 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cat::Cat()
	: type("Cat"), brain(new Brain())
{
	std::cout << "Cat default constructor called." << std::endl;
}

Cat::Cat(const Cat &src)
	: Animal(src), type(src.type), brain(new Brain(*src.brain))
{
	std::cout << "Cat copy constructor called." << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Cat::~Cat()
{
	std::cout << "Cat destructor called." << std::endl;
	delete this->brain;
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
		if (this->brain)
			delete this->brain;
		this->brain = new Brain(*rhs.brain);
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

std::string Cat::getIdea(int i)
{
	if (!this->brain)
		return "No brain";
	return this->brain->getIdea(i);
}

void Cat::setIdea(int i, std::string idea)
{
	if (this->brain)
		this->brain->setIdea(i, idea);
}

std::string const &Cat::getType(void) const
{
	return (this->type);
}

void Cat::setType(std::string const &type)
{
	this->type = type;
}

/* ************************************************************************** */
