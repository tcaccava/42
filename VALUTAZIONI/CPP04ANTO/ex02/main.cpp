/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:12:42 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 10:19:04 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"
#include "Dog.hpp"
#include "WrongCat.hpp"
#include "WrongDog.hpp"
#include <sstream>

const int arraySize = 10;

int	main( void ) {
	const Animal*	dog = new Dog();
	const Animal*	cat = new Cat();
	Animal* 		animals[arraySize];

	std::cout << "\nType: " << dog->getType() << std::endl;
	std::cout << "Type: " << cat->getType() << std::endl;

	cat->makeSound();
	dog->makeSound();

	delete cat;
	delete dog;

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

	Dog* smartDog = new Dog();
	Cat* smartCat = new Cat();

	smartDog->getBrain()->setIdea("Distruggi il giardino");
	smartDog->getBrain()->setIdea("Insegui il gatto");
	smartDog->getBrain()->setIdea("Caccia il gatto da casa");
	smartDog->getBrain()->setIdea("Cibo");
	smartDog->getBrain()->setIdea("Ancora cibo");

	smartCat->getBrain()->setIdea("Dormire");
	smartCat->getBrain()->setIdea("Svegliare l'umano per chiedergli del cibo");
	smartCat->getBrain()->setIdea("Dominare il mondo");
	smartCat->getBrain()->setIdea("Dormire");
	smartCat->getBrain()->setIdea("Dominare il mondo dormendo");

	std::cout << "\nDog's ideas:" << std::endl;
	for (int i = 0; i < 5; i++) {
		std::cout << "Idea " << i << ": " << smartDog->getBrain()->getIdea(i) << std::endl;
	}

	std::cout << "\nCat's ideas:" << std::endl;
	for (int i = 0; i < 5; i++) {
		std::cout << "Idea " << i << ": " << smartCat->getBrain()->getIdea(i) << std::endl;
	}

	Dog* thinkingDog = new Dog();

	for (int i = 0; i < 100; i++) {
		std::ostringstream oss;
		oss << "Idea number " << (i + 1);
		thinkingDog->getBrain()->setIdea(oss.str());
	}

	std::cout << "Brain is now full: " << (thinkingDog->getBrain()->isFull() ? "Yes" : "No") << std::endl;
	std::cout << "First idea: " << thinkingDog->getBrain()->getIdea(0) << std::endl;
	std::cout << "Last idea: " << thinkingDog->getBrain()->getIdea(99) << std::endl;

	thinkingDog->getBrain()->setIdea("Overflow idea");
	std::cout << "After adding overflow idea:" << std::endl;
	std::cout << "New first idea: " << thinkingDog->getBrain()->getIdea(0) << std::endl;

	std::cout << "\nCleaning up smart animals:" << std::endl;
	delete smartDog;
	delete smartCat;
	delete thinkingDog;

	return (0);
}
