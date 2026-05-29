/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonacco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 21:34:47 by ebonacco          #+#    #+#             */
/*   Updated: 2026/03/26 21:34:49 by ebonacco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "Brain.hpp"

int main()
{
    Animal* animals[10];

    for (int i = 0; i < 5; i++)
        animals[i] = new Dog();

    for (int i = 5; i < 10; i++)
        animals[i] = new Cat();

    for (int i = 0; i < 10; i++)
        delete animals[i];

    Dog original;

    for (int i = 0; i < 100; i++)
        original.getBrain()->setIdea(i, "Original idea");

    Dog copy = original;

    copy.getBrain()->setIdea(0, "Modified idea");

    std::cout << "Original[0]: " << original.getBrain()->getIdea(0) << std::endl;
    std::cout << "Copy[0]: " << copy.getBrain()->getIdea(0) << std::endl;

    Dog assigned;
    assigned = original;

    assigned.getBrain()->setIdea(1, "Assigned idea");

    std::cout << "Original[1]: " << original.getBrain()->getIdea(1) << std::endl;
    std::cout << "Assigned[1]: " << assigned.getBrain()->getIdea(1) << std::endl;

    return 0;
}