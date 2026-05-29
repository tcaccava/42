/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Character.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:14:28 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:14:29 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Character.hpp"

Character::Character( void ) : name("(null)") {
	for (int i = 0; i < MAX_MATERIAS; i++) {
		materias[i] = NULL;
	}
}

Character::Character( const std::string& newName ) : name(newName) {
	for (int i = 0; i < MAX_MATERIAS; i++) {
		materias[i] = NULL;
	}
}

Character::Character( const Character& character ) {
	name = character.name;
	for (int i = 0; i < MAX_MATERIAS; i++) {
		if (character.materias[i]) {
			materias[i] = character.materias[i]->clone();
		} else {
			materias[i] = NULL;
		}
	}
}

Character&	Character::operator=( const Character& character ) {
	if (this != &character) {
		name = character.name;
		for (int i = 0; i < MAX_MATERIAS; i++) {
			delete materias[i];
			if (character.materias[i]) {
				materias[i] = character.materias[i]->clone();
			} else {
				materias[i] = NULL;
			}
		}
	}
	return (*this);
}

const std::string&	Character::getName( void ) const {
	return (name);
}

void	Character::equip(AMateria* m) {
	for (int i = 0; i < MAX_MATERIAS; i++) {
		if (materias[i] == NULL) {
			materias[i] = m;
			return ;
		}
	}
	std::cout << "Cannot equip a materia! No more slots available" << std::endl;
}

void Character::unequip( int idx ) {
	if (idx < 0 || idx > (MAX_MATERIAS - 1)) {
		std::cout << "Cannot unequip a materia from a non existing slot!" << std::endl;
		return ;
	}
	materias[idx] = NULL;
}

void	Character::use( int idx, ICharacter& target ) {
	if (idx >= 0 && idx < MAX_MATERIAS && materias[idx]) {
		materias[idx]->use(target);
	}
}

Character::~Character( void ) {
	for (int i = 0; i < MAX_MATERIAS; i++) {
		delete materias[i];
	}
}
