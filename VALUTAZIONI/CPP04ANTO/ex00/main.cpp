/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:12:42 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 15:48:25 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"
#include "Dog.hpp"
#include "WrongCat.hpp"
#include "WrongDog.hpp"

int	main( void ) {
	const Animal* meta = new Animal();
	const Animal* dog = new Dog();
	const Animal* cat = new Cat();
	const WrongAnimal* metaWrong = new WrongAnimal();
	const WrongAnimal* dogWrong = new WrongDog();
	const WrongAnimal* catWrong = new WrongCat();

	meta->makeSound();
	meta->getType();
	dog->makeSound();
	dog->getType();
	cat->makeSound();
	dog->getType();
	metaWrong->makeSound();
	dogWrong->makeSound();
	catWrong->makeSound();

	delete cat;
	delete dog;
	delete meta;
	delete catWrong;
	delete dogWrong;
	delete metaWrong;

	return (0);
}
