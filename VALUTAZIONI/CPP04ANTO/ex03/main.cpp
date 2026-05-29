/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:30:23 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:32:01 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMateria.hpp"
#include "Character.hpp"
#include "Cure.hpp"
#include "Ice.hpp"
#include "MateriaSource.hpp"
#include "IMateriaSource.hpp"
#include "ICharacter.hpp"

int main()
{
	IMateriaSource* src = new MateriaSource();
	src->learnMateria(new Ice());
	src->learnMateria(new Cure());

	ICharacter* me = new Character("me");

	AMateria* tmp;
	tmp = src->createMateria("ice");
	me->equip(tmp);
	tmp = src->createMateria("cure");
	me->equip(tmp);

	ICharacter* bob = new Character("bob");

	me->use(0, *bob);
	me->use(1, *bob);

	// Unequipping a slot and trying to use it
	me->unequip(0);
	me->use(0, *bob); // Should not do anything

	// Attempt to equip more than 4 materias
	tmp = src->createMateria("cure");
	me->equip(tmp);
	tmp = src->createMateria("ice");
	me->equip(tmp);
	tmp = src->createMateria("ice");
	me->equip(tmp); // Should fail silently, inventory full

	// Attempt to use invalid slots
	me->use(-1, *bob); // Invalid index
	me->use(42, *bob); // Invalid index

	// Learn more than 4 materias in source
	src->learnMateria(new Ice());
	src->learnMateria(new Cure());
	src->learnMateria(new Ice()); // Should not be learned, source is full

	// Creating unknown materia
	AMateria* unknown = src->createMateria("fire");
	if (!unknown)
		std::cout << "Unknown materia 'fire' could not be created." << std::endl;

	delete bob;
	delete me;
	delete src;

	return 0;
}
