/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:33:02 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:07:07 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"
#include "Cat.hpp"
#include "Dog.hpp"
int main()
{
    std::cout << "----- CONSTRUCTORS -----" << std::endl;
    const Animal *animal = new Animal();
    const Animal *dog = new Dog();
    const Animal *cat = new Cat();

    std::cout << std::endl;
    const WrongAnimal *wrong_animal = new WrongAnimal();
    const WrongAnimal *wrong_cat = new WrongCat();
    const WrongCat *wrong_cat2 = new WrongCat();

    std::cout << std::endl;
    std::cout << "\n----- TYPES -----" << std::endl;
    std::cout << "Type: " <<  animal->getType() << std::endl;
    std::cout << "Type: " <<  dog->getType() << std::endl;
    std::cout << "Type: " <<  cat->getType() << std::endl;
    
    std::cout << std::endl;
    std::cout << "Type: " <<  wrong_animal->getType() << std::endl;
    std::cout << "Type: " <<  wrong_cat->getType() << std::endl;
    std::cout << "Type: " <<  wrong_cat2->getType() << std::endl;

    std::cout << std::endl;
    std::cout << "\n----- SOUNDS ------" << std::endl;
    animal->makeSound();
    dog->makeSound();
    cat->makeSound();

    std::cout << std::endl;
    wrong_animal->makeSound();
    wrong_cat->makeSound();
    wrong_cat2->makeSound();
    
    std::cout << std::endl;
    std::cout << "\n----- DESTRUCTORS -----" << std::endl;
    delete wrong_animal;
    delete wrong_cat;
    delete wrong_cat2;

    std::cout << std::endl;
    delete animal;
    delete dog;
    delete cat;
    return 0;
}