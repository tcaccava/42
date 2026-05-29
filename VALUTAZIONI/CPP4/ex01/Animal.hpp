/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:19 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 19:09:27 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <iostream>
#include <string>

class Animal
{
protected:
	std::string type;

public:
	Animal();
	Animal(Animal const &src);
	virtual ~Animal();
	Animal &operator=(Animal const &rhs);
	virtual void makeSound(void) const;
	const std::string getType(void) const;
	void setType(std::string const &type);
};

#endif
