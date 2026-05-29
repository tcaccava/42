/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongCat.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:05:43 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 15:42:53 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WrongCat.hpp"

WrongCat::WrongCat( void ) : WrongAnimal() {
	std::cout << "WrongCat default constructor called" << std::endl;
	type = "WrongCat";
}

WrongCat::WrongCat( const std::string& newName ): WrongAnimal(newName) {
	std::cout << "WrongCat constructor with parameters called" << std::endl;
	type = "WrongCat";
}

WrongCat::WrongCat( const WrongCat& WrongCat ) : WrongAnimal(WrongCat) {
	std::cout << "WrongCat copy constructor called" << std::endl;
	type = "WrongCat";
}

WrongCat&	WrongCat::operator=( const WrongCat& WrongCat ) {
	std::cout << "WrongCat copy assignment operator called" << std::endl;
	if (this != &WrongCat) {
		WrongAnimal::operator=(WrongCat);
	}
	return (*this);
}

void	WrongCat::makeSound( void ) const {
	std::cout << "WrongCat: MIAO MIAO!" << std::endl;
}

WrongCat::~WrongCat( void ) {
	std::cout << "WrongCat destructor called" << std::endl;
}
