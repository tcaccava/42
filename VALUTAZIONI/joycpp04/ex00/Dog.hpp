/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:32:57 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:15:04 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOG_HPP
#define DOG_HPP

#include "Animal.hpp"
#include <iostream>

class Dog: public Animal 
{
    public:
        Dog();
        Dog(const Dog &instance);
        Dog& operator=(const Dog &instance);
        ~Dog();
        
        void makeSound() const;
        std::string getType() const;
};

#endif