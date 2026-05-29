/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:56:52 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/09 09:25:41 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HumanA.hpp"

HumanA::HumanA(const std::string& newName, Weapon& newWeapon) : name(newName), weapon(newWeapon) {

}

void HumanA::attack( void ) {
	std::cout << name << " attacks with their " << weapon.getType() << std::endl;
}

HumanA::~HumanA() {

}
