/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:34:28 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:16:21 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOG_HPP
#define DOG_HPP

#include "Animal.hpp"
#include "Brain.hpp"
#include <iostream>

class Dog: public Animal 
{
    private:
        Brain *brain;
    public:
        Dog();
        Dog(const Dog &instance);
        Dog& operator=(const Dog &instance);
        ~Dog();
        
        void makeSound() const;
        std::string getType() const;
        
        Brain* getBrain() const;
};

#endif