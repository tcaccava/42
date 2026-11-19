/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 02:38:59 by ilnassi           #+#    #+#             */
/*   Updated: 2026/11/19 12:52:32 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AAnimal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"

int	main()
{
	std::cout << "\n -Abstract Class Polymorphism Test-" << std::endl;
	const AAnimal* j = new Dog();
	const AAnimal* i = new Cat();

	std::cout << j->getType() << " " << std::endl;
	std::cout << i->getType() << " " << std::endl;

	j->makeSound();
	i->makeSound();

	delete j;
	delete i;

	/*std::cout << "\n -Instantiation Test-" << std::endl;
	const AAnimal* testAbstract = new AAnimal();*/

	return 0;
}

