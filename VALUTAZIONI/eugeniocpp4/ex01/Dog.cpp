/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonacco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 21:36:31 by ebonacco          #+#    #+#             */
/*   Updated: 2026/03/26 21:36:33 by ebonacco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Dog.hpp"

Dog::Dog() : Animal("Dog")
{
	brain = new Brain();
	std::cout << "Dog Default constructor" << std::endl;
}

Dog::Dog(const Dog &obj) : Animal(obj)
{
	brain = new Brain(*obj.brain);
	std::cout << "Dog Copy constructor called" << std::endl;
}

Dog &Dog::operator=(const Dog &obj)
{
    if (this != &obj)
    {
        Animal::operator=(obj);
        delete brain;
        brain = new Brain(*obj.brain);
    }
    return *this;
}

Dog::~Dog()
{
	std::cout << "Destructor Dog" << std::endl;
	delete brain;
}

void Dog::makeSound() const
{
	std::cout << "Bau! Bau! Bau!... Bau!" << std::endl;
}

Brain* Dog::getBrain() const
{
    return brain;
}