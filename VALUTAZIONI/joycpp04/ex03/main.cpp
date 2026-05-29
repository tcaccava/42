/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:37:43 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:13:48 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMateria.hpp"
#include "Character.hpp"
#include "Cure.hpp"
#include "ICharacter.hpp"
#include "Ice.hpp"
#include "MateriaSource.hpp"

int main()
{
    std::cout << "\n--- CREATING MATERIASOURCE AND LEARNING MATERIAS ---"<< std::endl;
    IMateriaSource* src = new MateriaSource();
    src->learnMateria(new Ice());
    src->learnMateria(new Cure());

    std::cout << "--- CREATING CHARACTERS ---" << std::endl;
    ICharacter* me = new Character("me");
    ICharacter* bob = new Character("bob");
    ICharacter* meggie = new Character("meggie");

    std::cout << "--- CREATING AND EQUIPPING MATERIAS ---" << std::endl;
    AMateria* tmp;
    tmp = src->createMateria("ice");  me->equip(tmp);
    tmp = src->createMateria("cure"); me->equip(tmp);
    tmp = src->createMateria("ice");  me->equip(tmp);
    tmp = src->createMateria("cure"); me->equip(tmp);

    std::cout << "\n--- USING MATERIAS ON BOB ---" << std::endl;
    for (int i = 0; i < 4; ++i)
        me->use(i, *bob);

    std::cout << "\n--- UNEQUIPPING ALL SLOTS ---" << std::endl;
    for (int i = 0; i < 4; ++i)
        me->unequip(i);

    std::cout << "--- RE-EQUIPPING NEW MATERIAS ---" << std::endl;
    tmp = src->createMateria("ice");  me->equip(tmp);
    tmp = src->createMateria("cure"); me->equip(tmp);
    tmp = src->createMateria("ice");  me->equip(tmp);
    tmp = src->createMateria("cure"); me->equip(tmp);

    std::cout << "\n--- USING MATERIAS ON MEGGIE ---" << std::endl;
    for (int i = 0; i < 4; ++i)
        me->use(i, *meggie);

    std::cout << "\n--- EXTRA TESTS ---" << std::endl;

    std::cout << "TEST Creating unknown materia: ";
    tmp = src->createMateria("fire");
    if (!tmp)
        std::cout << "Unknown materia type correctly returned nullptr" << std::endl;

    std::cout << "TEST Trying to equip 5th materia (should do nothing): ";
    tmp = src->createMateria("ice");
    me->equip(tmp); 

    std::cout << "TEST Using invalid indices:" << std::endl;
    me->use(10, *bob);
    me->use(-1, *bob);
    me->unequip(10);
    me->unequip(-2);

    std::cout << "\n--- CLEANING UP ---" << std::endl;
    delete bob;
    delete meggie;
    delete me;
    delete src;

    return 0;
}
