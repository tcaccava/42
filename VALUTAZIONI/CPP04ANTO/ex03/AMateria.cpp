/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMateria.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:14:23 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:30:03 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMateria.hpp"

AMateria::AMateria( void ) : type("null") {

}

AMateria::AMateria( std::string const & newType ) : type(newType == "" ? "(null)" : newType) {

}

AMateria::AMateria( const AMateria& aMateria ) {
	type = aMateria.type;
}

AMateria&	AMateria::operator=( const AMateria& aMateria ) {
	(void)aMateria;
	std::cerr << "Assignment in AMateria class is not permitted" << std::endl;
	return (*this);
}

void	AMateria::use( ICharacter& target ) {
	(void) target;
}

std::string const& 	AMateria::getType() const {
	return (type);
}

AMateria::~AMateria() {
	// Default destructor
}
