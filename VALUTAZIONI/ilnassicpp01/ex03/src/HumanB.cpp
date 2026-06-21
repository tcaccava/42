/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 18:41:31 by ilnassi           #+#    #+#             */
/*   Updated: 2026/06/01 18:58:25 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HumanB.hpp"
#include "Weapon.hpp"
#include <iostream>

/*HumanB constractor.
Inizializza un oggetto HumanB con un nome specificato e
imposta il puntatore all'arma su NULL. 
** name: il nome di HumanB. 
** A differenza di HumanA, HumanB non richiede un'arma 
** al momento della creazione. 
** Un'arma può essere assegnata in seguito tramite setWeapon*/
HumanB::HumanB(std::string name) : _name(name), _weaponPTR(NULL) {}

HumanB::~HumanB() {}

void    HumanB::attack() const
{
    if (this->_weaponPTR != NULL)
        std::cout << this->_name << " attacks with their " << this->_weaponPTR->getType() << std::endl;
    else
        std::cout << this->_name << " has no weapon!" << std::endl;
    return;
}

void HumanB::setWeapon(Weapon& weapon)
{
    this->_weaponPTR = &weapon;
}
