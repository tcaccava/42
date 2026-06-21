/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 02:29:40 by ilnassi           #+#    #+#             */
/*   Updated: 2026/06/01 18:40:35 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HumanA.hpp"
#include "Weapon.hpp"
#include <iostream>

HumanA::HumanA(std::string name, Weapon& weapon) : _name(name), _weaponREF(weapon) {}

HumanA::~HumanA() {}

void    HumanA::attack() const
{
    std::cout << this->_name << " attacks with their: " << this->_weaponREF.getType() << std::endl;
}
