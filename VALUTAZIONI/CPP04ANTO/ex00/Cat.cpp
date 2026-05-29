/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:05:43 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:25:47 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"

Cat::Cat( void ) : Animal() {
	std::cout << "Cat default constructor called" << std::endl;
	type = "Cat";
}

Cat::Cat( const Cat& cat ) : Animal(cat) {
	std::cout << "Cat copy constructor called" << std::endl;
}

Cat&	Cat::operator=( const Cat& cat ) {
	std::cout << "Cat copy assignment operator called" << std::endl;
	if (this != &cat) {
		Animal::operator=(cat);
	}
	return (*this);
}

void	Cat::makeSound( void ) const {
	std::cout << "oooooOH...MIAO" << std::endl;
}

Cat::~Cat( void ) {
	std::cout << "Cat destructor called" << std::endl;
}
