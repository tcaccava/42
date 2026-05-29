/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:34:34 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:06:49 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"
#include "Cat.hpp"
#include "Dog.hpp"

int main()
{
    std::cout << "\n----- COPY CAT -----" << std::endl;
    Cat cat1;
    Cat cat2(cat1);

    std::cout << "\n----- ARRAY OF ANIMALS -----" << std::endl;
    Animal *animal[10];
    for(int i=0; i<10; i++)
    {
        if(i % 2)
            animal[i] = new Cat();
        else
            animal[i] = new Dog();
    }

    std::cout << "\n----- TYPES -----" << std::endl;
    for(int i=0; i<10; i++)
        std::cout << animal[i]->getType() << std::endl;

    std::cout << "\n----- BRAIN DEEP COPY TEST -----" << std::endl;
    Dog dog1;
    dog1.getBrain()->setIdea(0, "I want a ball!");
    
    Dog dog2 = dog1;

    std::cout << "Dog1 idea: " << dog1.getBrain()->getIdea(0) << std::endl;
    std::cout << "Dog2 idea: " << dog2.getBrain()->getIdea(0) << std::endl;

    dog2.getBrain()->setIdea(0, "I want to sleep...");

    std::cout << "After modification:" << std::endl;
    std::cout << "Dog1 idea: " << dog1.getBrain()->getIdea(0) << std::endl;
    std::cout << "Dog2 idea: " << dog2.getBrain()->getIdea(0) << std::endl;

    std::cout << "\n----- DEEP COPY -----" << std::endl;
    Dog basic;
    {
        Dog tmp = basic;
    }

    std::cout << "\n----- DELETING ANIMAL ARRAY -----" << std::endl;
    for(int i=0; i<10; i++)
        delete animal[i];

    return 0;
}
