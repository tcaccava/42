/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:57:00 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/09 09:21:09 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HumanB.hpp"

HumanB::HumanB(const std::string& newName) : name(newName), weapon(nullptr) {

}

void HumanB::attack( void ) {
	std::cout << name << " attacks with their " << weapon->getType() << std::endl;
}

HumanB::~HumanB() {

}
