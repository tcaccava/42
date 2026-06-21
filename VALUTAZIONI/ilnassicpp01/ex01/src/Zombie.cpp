/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 16:35:07 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/30 16:35:09 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Zombie.hpp"

Zombie::Zombie() : _name("unnamed") {}

Zombie::Zombie(std::string name) : _name(name) {}

Zombie::~Zombie() 
{
    std::cout << _name << " is destroyed." << std::endl;
}

void Zombie::setName(std::string name)
{
    _name = name;
}

void Zombie::announce()
{
    std::cout << _name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}
