/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:19 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 19:46:51 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <iostream>
#include <string>

class AAnimal
{
protected:
	std::string type;

public:
	AAnimal();
	AAnimal(AAnimal const &src);
	virtual ~AAnimal();
	AAnimal &operator=(AAnimal const &rhs);
	virtual void makeSound(void) const = 0;
	const std::string getType(void) const;
	void setType(std::string const &type);
};

#endif
