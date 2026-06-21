/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 18:58:52 by ilnassi           #+#    #+#             */
/*   Updated: 2026/06/01 19:23:31 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Weapon.hpp"

Weapon::Weapon(std::string type) : _type(type){}

Weapon::~Weapon() {}

const std::string& Weapon::getType(void) const
{
    return _type;
}

void    Weapon::setType(std::string type)
{
    _type = type;
}
