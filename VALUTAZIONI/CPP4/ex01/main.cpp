/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:56:38 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 19:40:26 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include "Animal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"

#define N_ANIMALS 100

int main()
{
	{
		std::cout << "Test Brain deep copy" << std::endl;

		Cat cat1;
		Cat cat2;
		cat1.setIdea(5, "ABCDEF");
		Cat *cat3 = new Cat(cat1);
		cat1.setIdea(5, "GHIJK");
		cat2 = cat1;
		cat2.setIdea(5, "XYZ");
		Cat *cat4 = new Cat(*cat3);
		std::cout << "Cat1 Idea: " << cat1.getIdea(5) << std::endl;
		std::cout << "Cat2 Idea: " << cat2.getIdea(5) << std::endl;
		std::cout << "Cat3 Idea: " << cat3->getIdea(5) << std::endl;
		delete cat3;
		std::cout << "Cat4 Idea: " << cat4->getIdea(5) << std::endl;
		delete cat4;

		Dog dog1;
		Dog dog2;
		dog1.setIdea(5, "ABCDEF");
		Dog *dog3 = new Dog(dog1);
		dog1.setIdea(5, "GHIJK");
		dog2 = dog1;
		dog2.setIdea(5, "XYZ");
		Dog *dog4 = new Dog(*dog3);
		std::cout << "Dog1 Idea: " << dog1.getIdea(5) << std::endl;
		std::cout << "Dog2 Idea: " << dog2.getIdea(5) << std::endl;
		std::cout << "Dog3 Idea: " << dog3->getIdea(5) << std::endl;
		delete dog3;
		std::cout << "Dog4 Idea: " << dog4->getIdea(5) << std::endl;
		delete dog4;
	}
	std::cout << std::endl;
	{
		std::cout << "Test Array" << std::endl;
		Animal *animals[N_ANIMALS];
		Cat cat;
		cat.setIdea(5, "Cat Idea");
		Dog dog;
		dog.setIdea(5, "Dog Idea");

		for (int i = 0; i < N_ANIMALS; i++)
		{
			if (i & 1)
			{
				Cat *catCopy = new Cat(cat);
				catCopy->setIdea(70, "Cat");
				animals[i] = catCopy;
			}
			else
			{
				Dog *dogCopy = new Dog(dog);
				dogCopy->setIdea(70, "Dog");
				animals[i] = dogCopy;
			}
		}
		for (int i = 0; i < N_ANIMALS; i++)
		{
			std::cout << "Animal " << i << ": " << animals[i]->getType() << std::endl;
			if (i & 1)
			{
				std::cout << '\t' << ((Cat *)animals[i])->getType() << std::endl;
				std::cout << '\t' << ((Cat *)animals[i])->getIdea(5) << std::endl;
				std::cout << '\t' << ((Cat *)animals[i])->getIdea(70) << std::endl;
			}
			else
			{
				std::cout << '\t' << ((Dog *)animals[i])->getType() << std::endl;
				std::cout << '\t' << ((Dog *)animals[i])->getIdea(5) << std::endl;
				std::cout << '\t' << ((Dog *)animals[i])->getIdea(70) << std::endl;
			}
			animals[i]->makeSound();
			std::cout << std::endl;
		}
		for (int i = 0; i < N_ANIMALS; i++)
			delete animals[i];
	}
	{
		std::cout << "Test Value Instances" << std::endl;
		Animal animal;
		Cat cat;
		Dog dog;
		Cat catCopy(cat);
		Dog dogCopy(dog);
		Animal animalCopy(cat);

		std::cout << "Animal: " << animal.getType() << std::endl;
		animal.makeSound();
		std::cout << "Cat: " << cat.getType() << std::endl;
		cat.makeSound();
		std::cout << "Dog: " << dog.getType() << std::endl;
		dog.makeSound();
		std::cout << "CatCopy: " << catCopy.getType() << std::endl;
		catCopy.makeSound();
		std::cout << "DogCopy: " << dogCopy.getType() << std::endl;
		dogCopy.makeSound();
		std::cout << "AnimalCopy: " << animalCopy.getType() << std::endl;
		animalCopy.makeSound();
		std::cout << std::endl;
	}
	std::cout << std::endl;
	{
		std::cout << "Test Pointer Instances" << std::endl;
		Animal *animal = new Animal();
		Cat *cat = new Cat();
		Dog *dog = new Dog();
		Cat *catCopy = new Cat(*cat);
		Dog *dogCopy = new Dog(*dog);
		Animal *animalCopy = new Animal(*cat);
		Animal *animalCatCopy = new Cat(*cat);
		std::cout << "Animal: " << animal->getType() << std::endl;
		animal->makeSound();
		std::cout << "Cat: " << cat->getType() << std::endl;
		cat->makeSound();
		std::cout << "Dog: " << dog->getType() << std::endl;
		dog->makeSound();
		std::cout << "CatCopy: " << catCopy->getType() << std::endl;
		catCopy->makeSound();
		std::cout << "DogCopy: " << dogCopy->getType() << std::endl;
		dogCopy->makeSound();
		std::cout << "AnimalCopy: " << animalCopy->getType() << std::endl;
		animalCopy->makeSound();
		std::cout << "AnimalCatCopy: " << animalCatCopy->getType() << std::endl;
		animalCatCopy->makeSound();
		std::cout << std::endl;
		delete animal;
		delete cat;
		delete dog;
		delete catCopy;
		delete dogCopy;
		delete animalCopy;
		delete animalCatCopy;
	}
	std::cout << std::endl;
	{
		std::cout << "Test Animal Pointer Instances" << std::endl;
		Animal *animal = new Animal();
		Animal *cat = new Cat();
		Animal *dog = new Dog();
		Animal *catCopy = new Animal(*cat);
		Animal *dogCopy = new Animal(*dog);
		std::cout << "Animal: " << animal->getType() << std::endl;
		animal->makeSound();
		std::cout << "Cat: " << cat->getType() << std::endl;
		cat->makeSound();
		std::cout << "Dog: " << dog->getType() << std::endl;
		dog->makeSound();
		std::cout << "CatCopy: " << catCopy->getType() << std::endl;
		catCopy->makeSound();
		std::cout << "DogCopy: " << dogCopy->getType() << std::endl;
		dogCopy->makeSound();
		std::cout << std::endl;
		delete animal;
		delete cat;
		delete dog;
		delete catCopy;
		delete dogCopy;
	}
	std::cout << std::endl;
	return (0);
}
