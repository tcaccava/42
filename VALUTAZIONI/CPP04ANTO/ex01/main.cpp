/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:12:42 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 21:35:51 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"
#include "Dog.hpp"
#include "WrongCat.hpp"
#include "WrongDog.hpp"

const int arraySize = 10;

int	main( void ) {
	const Animal*	meta = new Animal();
	const Animal*	dog = new Dog();
	const Animal*	cat = new Cat();
	Animal* 		animals[arraySize];

	std::cout << "\nType: " << dog->getType() << std::endl;
	std::cout << "Type: " << cat->getType() << std::endl;

	cat->makeSound();
	dog->makeSound();
	meta->makeSound();

	delete cat;
	delete dog;
	delete meta;


	for (int i = 0; i < arraySize / 2; i++) {
		std::cout << "Creating Dog " << i << ":" << std::endl;
		animals[i] = new Dog();
	}

	for (int i = arraySize / 2; i < arraySize; i++) {
		std::cout << "Creating Cat " << i << ":" << std::endl;
		animals[i] = new Cat();
	}

	for (int i = 0; i < arraySize; i++) {
		std::cout << "Animal " << i << " (" << animals[i]->getType() << "): ";
		animals[i]->makeSound();
	}

	for (int i = 0; i < arraySize; i++) {
		std::cout << "Deleting Animal " << i << " (" << animals[i]->getType() << "):" << std::endl;
		delete animals[i];
	}

	Dog* originalDog = new Dog();
	Dog* copiedDog = new Dog(*originalDog);


	copiedDog->makeSound();

	delete originalDog;
	delete copiedDog;

	return (0);
}
