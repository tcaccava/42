/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongCat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:08:48 by rauer             #+#    #+#             */
/*   Updated: 2024/11/19 16:53:25 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRONGCAT_HPP
#define WRONGCAT_HPP
#include <iostream>
#include <string>
#include "WrongAnimal.hpp"

class WrongCat : public WrongAnimal
{
private:
	std::string type;

public:
	WrongCat();
	WrongCat(WrongCat const &src);
	~WrongCat();
	WrongCat &operator=(WrongCat const &rhs);
	void makeSound(void);
	std::string const &getType(void) const;
	void setType(std::string const &type);
};

#endif
