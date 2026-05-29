/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonacco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 21:36:41 by ebonacco          #+#    #+#             */
/*   Updated: 2026/03/26 21:36:43 by ebonacco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Cat.hpp"

Cat::Cat() : Animal("Cat")
{
	brain = new Brain();
	std::cout << "Cat Default constructor" << std::endl;
}

Cat::Cat(const Cat &obj) : Animal(obj)
{
	brain = new Brain(*obj.brain);
	std::cout << "Cat Copy constructor called" << std::endl;
}

Cat &Cat::operator=(const Cat &obj)
{
    if (this != &obj)
    {
        Animal::operator=(obj);
        delete brain;
        brain = new Brain(*obj.brain);
    }
    return *this;
}

Cat::~Cat()
{
	std::cout << "Destructor Cat" << std::endl;
	delete brain;
}

void Cat::makeSound() const
{
	std::cout << "Meooooooooaw..." << std::endl;
}

Brain* Cat::getBrain() const
{
    return brain;
}