/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:36:05 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:09:52 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"
#include "Cat.hpp"
#include "Dog.hpp"

int main()
{
    std::cout << "\n----- CONSTRUCTORS -----" << std::endl;
    //Animal *animal = new Animal();
    const Animal *dog = new Dog();
    const Animal *cat = new Cat();

    std::cout << std::endl;
    std::cout << "----- TYPES -----" << std::endl;
    //std::cout << "Type: " <<  animal->getType() << std::endl;
    std::cout << "Type: " << dog->getType() << std::endl;
    std::cout << "Type: " << cat->getType() << std::endl;

    std::cout << std::endl;
    std::cout << "----- SOUNDS -----" << std::endl;
    //animal->makeSound();
    dog->makeSound();
    cat->makeSound();

    std::cout << std::endl;
    std::cout << "----- DESTRUCTORS -----" << std::endl;
    //delete animal;
    delete dog;
    delete cat;

    return 0;
}