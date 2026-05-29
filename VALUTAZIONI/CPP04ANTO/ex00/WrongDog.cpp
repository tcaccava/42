/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongDog.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:50:34 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 15:42:53 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WrongDog.hpp"

WrongDog::WrongDog( void ) : WrongAnimal() {
	std::cout << "WrongDog default constructor called" << std::endl;
	type = "WrongDog";
}

WrongDog::WrongDog( const std::string& newName ) : WrongAnimal(newName) {
	std::cout << "WrongDog constructor with parameters called" << std::endl;
	type = "WrongDog";
}

WrongDog::WrongDog( const WrongDog& WrongDog ) : WrongAnimal(WrongDog) {
	std::cout << "WrongDog copy constructor called" << std::endl;
}

WrongDog&	WrongDog::operator=( const WrongDog& WrongDog ) {
	std::cout << "WrongDog copy assignment constructor called" << std::endl;
	if (this != &WrongDog) {
		WrongAnimal::operator=(WrongDog);
	}
	return (*this);
}

void	WrongDog::makeSound( void ) const {
	std::cout << "WrongDog: WOOF WOOF!" << std::endl;
}

WrongDog::~WrongDog( void ) {
	std::cout << "WrongDog destructor called" << std::endl;
}
