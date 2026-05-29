/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonacco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 21:34:47 by ebonacco          #+#    #+#             */
/*   Updated: 2026/03/26 21:34:49 by ebonacco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Animal.hpp"
#include "WrongAnimal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongCat.hpp"

int	main() {
	const Animal* meta = new Animal();
	const Animal* j = new Dog();
	const Animal* i = new Cat();

	std::cout << j->getType() << " " << std::endl;
	std::cout << i->getType() << " " << std::endl;
	i->makeSound(); //will output the cat sound!
	j->makeSound();
	meta->makeSound();

	const WrongAnimal* a = new WrongAnimal();
	const WrongAnimal* b = new WrongCat();
	const WrongCat *c = new WrongCat();
	const WrongCat effe;

	a->makeSound();
	b->makeSound();
	c->makeSound();
	effe.makeSound();
	delete meta;
	delete j;
	delete i;
	delete a;
	delete b;
	delete c;

	return 0;
}
