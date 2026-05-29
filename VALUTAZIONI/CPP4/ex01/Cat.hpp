/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:48 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 19:14:25 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAT_HPP
#define CAT_HPP

#include <iostream>
#include <string>
#include "Animal.hpp"
#include "Brain.hpp"

class Cat : public Animal
{
private:
	std::string type;
	Brain *brain;

public:
	Cat();
	Cat(Cat const &src);
	virtual ~Cat();
	Cat &operator=(Cat const &rhs);
	virtual void makeSound(void) const;
	std::string getIdea(int i);
	void setIdea(int i, std::string idea);
	std::string const &getType(void) const;
	void setType(std::string const &type);
};

#endif
