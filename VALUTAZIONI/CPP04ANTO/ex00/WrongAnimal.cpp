/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongAnimal.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:44:16 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 15:42:53 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal( void ) : name("(null)") , type("(null)") {
	std::cout << "WrongAnimal default constructor called" << std::endl;
}

WrongAnimal::WrongAnimal( const std::string& newName ) : name(newName == "" ? "(null)" : newName), type("(null)") {
	std::cout << "WrongAnimal constructor with parameters called" << std::endl;

}

WrongAnimal::WrongAnimal( const WrongAnimal& WrongAnimal ) {
	std::cout << "WrongAnimal copy constructor called" << std::endl;
	name = WrongAnimal.name;
	type = WrongAnimal.type;
}

WrongAnimal&	WrongAnimal::operator=( const WrongAnimal& WrongAnimal ) {
	std::cout << "WrongAnimal copy assignment constructor called" << std::endl;
	if (this != &WrongAnimal) {
		name = WrongAnimal.name;
		type = WrongAnimal.type;
	}
	return (*this);
}

void	WrongAnimal::makeSound( void ) const {
	std::cout << "WrongAnimal generic sound" << std::endl;
}

WrongAnimal::~WrongAnimal( void ) {
	std::cout << "WrongAnimal destructor called" << std::endl;
}
