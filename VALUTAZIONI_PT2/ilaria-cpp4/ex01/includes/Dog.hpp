/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 20:10:58 by ilnassi           #+#    #+#             */
/*   Updated: 2026/11/19 13:06:51 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOG_HPP
#define DOG_HPP

#include "Animal.hpp"
#include "Brain.hpp"

class Dog : public Animal 
{
	private:
		Brain* _brain;
	
	public:
		Dog();
		Dog(const Dog& other);
		Dog& operator=(const Dog& other);
		virtual ~Dog();
		
		virtual void makeSound() const;
		/*A constant getter method in C++ that returns a pointer to an object of type Brain.
		It is used to access a private attribute of a class without modifying 
		the state of the calling object. */
		Brain* getBrain() const;
};

#endif
