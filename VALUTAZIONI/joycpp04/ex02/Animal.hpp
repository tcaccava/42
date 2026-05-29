/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:35:28 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:17:03 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <iostream>

class Animal 
{
    protected:
        std::string type;
    public:
        Animal();
        Animal(std::string Type);
        Animal(const Animal &instance);
        Animal& operator=(const Animal &instance);
        virtual ~Animal() = 0;

        virtual void makeSound() const = 0;
        std::string getType() const;
};

#endif