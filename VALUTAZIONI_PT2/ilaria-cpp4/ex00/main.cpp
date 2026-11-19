/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:16:03 by ilnassi           #+#    #+#             */
/*   Updated: 2026/11/19 13:11:20 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"

int	main()
{
	std::cout << "\n* CORRECT POLYMORPHISM *" << std::endl;
	
	const Animal* meta = new Animal();
	const Animal* dog = new Dog();
	const Animal* cat = new Cat();
	
	std::cout << dog->getType() << std::endl;
	std::cout << cat->getType() << std::endl;
	
	cat->makeSound();
	dog->makeSound();
	meta->makeSound();
	
	delete meta;
	delete dog;
	delete cat;
	
	std::cout << "\n* WRONG POLYMORPHISM (NO VIRTUAL) *" << std::endl;
	
	/*Create an actual WrongCat object in memory, and then create a read-only generic pointer 
	(const WrongAnimal*) that points to the address of that cat. */
	WrongCat wrongCat;
	const WrongAnimal* wrongAnimal = &wrongCat;
	
	std::cout << wrongAnimal->getType() << std::endl;
	wrongAnimal->makeSound();
	
	std::cout << "\n* COPY CONSTRUCTOR AND ASSIGNMENT TESTS *" << std::endl;
	
	Dog dogA;
	/*Create a new dog called dogB by cloning it directly from dogA.*/
	Dog dogB(dogA);
	Dog dogC;
	/*Take the data from dogA and copy it into dogC*/
	dogC = dogA;
	
	Cat catA;
	Cat catB(catA);
	Cat catC;
	catC = catA;
	
	return 0;
}

