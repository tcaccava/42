/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMateria.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:36:46 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 11:59:50 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMateria.hpp"

AMateria::AMateria(std::string const &Type)
{
    type = Type;
}
AMateria::AMateria(const AMateria &instance) 
{
    type = instance.type;
}

AMateria& AMateria::operator=(const AMateria &instance) 
{
    if (this != &instance)
        type = instance.type;
    return *this;
}

std::string const &AMateria::getType() const
{
    return type;
}

AMateria::~AMateria()
{

}

void AMateria::use(ICharacter& target)
{
    std::cout << "* AMateria " << target.getName() << "'s use function *" << std::endl;
}