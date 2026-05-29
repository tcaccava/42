/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:56 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 19:14:35 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOG_HPP
#define DOG_HPP

#include <iostream>
#include <string>
#include "Animal.hpp"
#include "Brain.hpp"

class Dog : public Animal
{
private:
	std::string type;
	Brain *brain;

public:
	Dog();
	Dog(Dog const &src);
	virtual ~Dog();
	Dog &operator=(Dog const &rhs);
	virtual void makeSound(void) const;
	std::string getIdea(int i);
	void setIdea(int i, std::string idea);
	std::string const &getType(void) const;
	void setType(std::string const &type);
};

#endif
