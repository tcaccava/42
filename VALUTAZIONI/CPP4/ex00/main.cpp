/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:56:38 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 18:59:06 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"

int main()
{
	{
		std::cout << "Test Value Instances" << std::endl;
		Animal animal;
		Cat cat;
		Dog dog;
		Cat catCopy(cat);
		Dog dogCopy(dog);
		Animal animalCopy(cat);
		WrongAnimal wrongAnimal;
		WrongCat wrongCat;

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
		std::cout << "WrongAnimal: " << wrongAnimal.getType() << std::endl;
		wrongAnimal.makeSound();
		std::cout << "WrongCat: " << wrongCat.getType() << std::endl;
		wrongCat.makeSound();
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
		WrongAnimal *wrongAnimal = new WrongAnimal();
		WrongCat *wrongCat = new WrongCat();
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
		std::cout << "WrongAnimal: " << wrongAnimal->getType() << std::endl;
		wrongAnimal->makeSound();
		std::cout << "WrongCat: " << wrongCat->getType() << std::endl;
		wrongCat->makeSound();
		std::cout << std::endl;
		delete animal;
		delete cat;
		delete dog;
		delete catCopy;
		delete dogCopy;
		delete animalCopy;
		delete animalCatCopy;
		delete wrongAnimal;
		delete wrongCat;
	}
	std::cout << std::endl;
	{
		std::cout << "Test Animal Pointer Instances" << std::endl;
		Animal *animal = new Animal();
		Animal *cat = new Cat();
		Animal *dog = new Dog();
		Animal *catCopy = new Animal(*cat);
		Animal *dogCopy = new Animal(*dog);
		WrongAnimal *wrongAnimal = new WrongAnimal();
		WrongAnimal *wrongCat = new WrongCat();
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
		std::cout << "WrongAnimal: " << wrongAnimal->getType() << std::endl;
		wrongAnimal->makeSound();
		std::cout << "WrongCat: " << wrongCat->getType() << std::endl;
		wrongCat->makeSound();
		std::cout << std::endl;
		delete animal;
		delete cat;
		delete dog;
		delete catCopy;
		delete dogCopy;
		delete wrongAnimal;
		delete wrongCat;
	}
	std::cout << std::endl;
	return (0);
}
