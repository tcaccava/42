/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:48 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 18:54:47 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAT_HPP
#define CAT_HPP

#include <iostream>
#include <string>
#include "Animal.hpp"

class Cat : public Animal
{
private:
	std::string type;

public:
	Cat();
	Cat(Cat const &src);
	~Cat();
	Cat &operator=(Cat const &rhs);
	void makeSound(void) const;
	std::string const &getType(void) const;
	void setType(std::string const &type);
};

#endif
