/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:25:26 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 18:13:51 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ice.hpp"
#include "Cure.hpp"
#include "Character.hpp"
#include "MateriaSource.hpp"

int main()
{
	MateriaSource *src = new MateriaSource();
	src->learnMateria(new Ice());
	src->learnMateria(new Cure());
	IMateriaSource *src2 = new MateriaSource(*src);
	delete src;

	Character *me = new Character("me");
	AMateria *tmp;
	tmp = src2->createMateria("ice");
	me->equip(tmp);
	tmp = src2->createMateria("cure");
	me->equip(tmp);
	ICharacter *bob = new Character("bob");
	me->use(0, *bob);
	ICharacter *me2 = new Character(*me);
	delete me;
	me2->use(1, *bob);
	delete bob;
	delete me2;
	delete src2;

	return 0;
}
