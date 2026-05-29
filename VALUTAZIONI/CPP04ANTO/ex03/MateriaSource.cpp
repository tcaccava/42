/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MateriaSource.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:14:59 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:15:00 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MateriaSource.hpp"

MateriaSource::MateriaSource() {
	for (int i = 0; i < MAX_MATERIAS; i++) {
		materias[i] = NULL;
	}
}

MateriaSource::MateriaSource( const MateriaSource& materiaSource) {
	for (int i = 0; i < MAX_MATERIAS; i++) {
		if (materiaSource.materias[i]) {
			materias[i] = materiaSource.materias[i]->clone();
		} else {
			materias[i] = NULL;
		}
	}
}

MateriaSource&	MateriaSource::operator=( const MateriaSource& materiaSource ) {
	if (this != &materiaSource) {
		for (int i = 0; i < MAX_MATERIAS; i++) {
			if (materiaSource.materias[i]) {
				delete materias[i];
				materias[i] = materiaSource.materias[i]->clone();
			} else {
				materias[i] = NULL;
			}
		}
	}
	return *this;
}

void MateriaSource::learnMateria( AMateria* materia ) {
	for (int i = 0; i < MAX_MATERIAS; i++) {
		if (materias[i] == NULL && materia) {
			materias[i] = materia->clone();
			break ;
		}
	}
}

AMateria* MateriaSource::createMateria( std::string const & type ) {
	AMateria*	newMateria;

	if (type != "cure" && type != "ice") {
		return (0);
	}
	newMateria = 0;
	for (int i = 0; i < MAX_MATERIAS; i++) {
		if (materias[i] && materias[i]->getType() == type) {
			newMateria = materias[i]->clone();
			break ;
		}
	}
	return (newMateria);
}

MateriaSource::~MateriaSource() {
	for (int i = 0; i < MAX_MATERIAS; i++) {
		delete materias[i];
	}
}
