/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cure.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:14:36 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:14:37 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cure.hpp"

Cure::Cure( void ) : AMateria("cure") {

}

Cure::Cure( const Cure& cure ) : AMateria(cure) {

}

Cure&	Cure::operator=( const Cure& cure ) {
	AMateria::operator=(cure);
	return (*this);
}

AMateria* Cure::clone() const {
	return (new Cure(*this));
}

void	Cure::use( ICharacter& target ) {
	std::cout << "* heals " << target.getName() << "’s wounds *" << std::endl;
}

Cure::~Cure( void ) {

}
