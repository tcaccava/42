/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ice.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:37:21 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 12:25:32 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ice.hpp"
#include "AMateria.hpp"

Ice::Ice():AMateria("ice")
{

}

Ice::Ice(const Ice &instance):AMateria(instance.type)
{
    std::cout << "Ice copy constructor called" << std::endl;
}
Ice& Ice::operator=(const Ice &instance)
{
    if(this != &instance)
        type = instance.type;
    return *this;
}
Ice::~Ice()
{

}

AMateria* Ice::clone() const
{
    Ice *ice = new Ice();
    return ice;
}

void Ice::use(ICharacter& target)
{
    std::cout << "* shoots an ice bolt at " << target.getName() << " *" << std::endl;
}
