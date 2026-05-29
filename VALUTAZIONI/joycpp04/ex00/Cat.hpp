/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:32:43 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:14:49 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAT_HPP
#define CAT_HPP

#include "Animal.hpp"
#include <iostream>

class Cat: public Animal 
{
    public:
        Cat();
        Cat(const Cat &instance);
        Cat& operator=(const Cat &instance);
        ~Cat();
        
        void makeSound() const;
        std::string getType() const;
};

#endif