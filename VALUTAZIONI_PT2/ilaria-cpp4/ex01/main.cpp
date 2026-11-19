/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 01:08:39 by ilnassi           #+#    #+#             */
/*   Updated: 2026/11/19 12:56:37 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "Brain.hpp"

int	main()
{
	std::cout << "\n--- SUBJECT TEST ---" << std::endl;
	const Animal* j = new Dog();
	const Animal* i = new Cat();
	delete j;
	delete i;
	
	std::cout << "\n--- ANIMAL ARRAY TEST ---" << std::endl;
	const int arraySize = 4;
	Animal* animals[arraySize];
	
	for (int idx = 0; idx < arraySize / 2; idx++)
		animals[idx] = new Dog();
	for (int idx = arraySize / 2; idx < arraySize; idx++)
		animals[idx] = new Cat();
	
	std::cout << "\n--- ARRAY DESTRUCTION ---" << std::endl;
	for (int idx = 0; idx < arraySize; idx++)
		delete animals[idx];
	
	std::cout << "\n--- DOG COPY CONSTRUCTOR: DEEP COPY ---" << std::endl;
	Dog basicDog;
	basicDog.getBrain()->setIdea(0, "dog treat please!");
	Dog cloneDog(basicDog);
	
	std::cout << "Original: " << basicDog.getBrain()->getIdea(0) << std::endl;
	std::cout << "Copy: " << cloneDog.getBrain()->getIdea(0) << std::endl;
	
	cloneDog.getBrain()->setIdea(0, "I want to chase a cat!");
	
	std::cout << "Modified copy: " << cloneDog.getBrain()->getIdea(0) << std::endl;
	std::cout << "Original unchanged: " << basicDog.getBrain()->getIdea(0) << std::endl;
	
	std::cout << "\n--- DOG ASSIGNMENT: DEEP COPY ---" << std::endl;
	Dog dogA;
	Dog dogB;
	
	dogA.getBrain()->setIdea(0, "I love bones!");
	dogB.getBrain()->setIdea(0, "I love cats!");
	
	dogB = dogA;
	dogB.getBrain()->setIdea(0, "I changed my mind!");
	
	std::cout << "dogA: " << dogA.getBrain()->getIdea(0) << std::endl;
	std::cout << "dogB: " << dogB.getBrain()->getIdea(0) << std::endl;
	
	
	std::cout << "\n--- CAT COPY AND ASSIGNMENT TESTS ---" << std::endl;
	Cat catA;
	catA.getBrain()->setIdea(0, "I want some tuna!");
	
	Cat catB(catA);
	catB.getBrain()->setIdea(0, "I want a nap!");
	
	std::cout << "catA: " << catA.getBrain()->getIdea(0) << std::endl;
	std::cout << "catB: " << catB.getBrain()->getIdea(0) << std::endl;
	
	Cat catC;
	catC = catA;
	catC.getBrain()->setIdea(0, "I want to climb!");
	
	std::cout << "catA: " << catA.getBrain()->getIdea(0) << std::endl;
	std::cout << "catC: " << catC.getBrain()->getIdea(0) << std::endl;
	
	std::cout << "\n--- END OF MAIN ---" << std::endl;
	return 0;
}
