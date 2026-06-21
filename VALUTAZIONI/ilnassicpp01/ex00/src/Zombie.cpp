/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 16:55:54 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/30 16:55:56 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Zombie.hpp"

/*Constructor: initializes the Zombie's name 
using a member initialization list.*/
Zombie::Zombie(std::string name) : _name(name) {}

/*Destructor: called automatically when the object goes out of scope (stack)
or is explicitly deleted (heap). 
Prints a message so we can SEE when it happens.*/
Zombie::~Zombie()
{
    std::cout << _name << ": is destroyed" << std::endl;
}

void Zombie::announce(void)
{
    std::cout << _name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}
