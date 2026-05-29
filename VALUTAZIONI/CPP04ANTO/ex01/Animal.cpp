/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:44:16 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:25:47 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"

Animal::Animal( void ) : type("Animal") {
	std::cout << "Animal default constructor called" << std::endl;
}

Animal::Animal( const Animal& animal ) {
	std::cout << "Animal copy constructor called" << std::endl;
	type = animal.type;
}

Animal&	Animal::operator=( const Animal& animal ) {
	std::cout << "Animal copy assignment operator called" << std::endl;
	if (this != &animal) {
		type = animal.type;
	}
	return (*this);
}

const std::string&	Animal::getType( void ) const {
	return (type);
}

void	Animal::makeSound( void ) const {
	std::cout << "Animal makes a generic sound" << std::endl;
}

Animal::~Animal( void ) {
	std::cout << "Animal destructor called" << std::endl;
}
