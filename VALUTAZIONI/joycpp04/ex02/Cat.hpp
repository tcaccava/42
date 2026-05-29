/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:35:52 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:17:21 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAT_HPP
#define CAT_HPP

#include "Animal.hpp"
#include "Brain.hpp"
#include <iostream>

class Cat: public Animal 
{
    private:
        Brain *brain;
    public:
        Cat();
        Cat(const Cat &instance);
        Cat& operator=(const Cat &instance);
        ~Cat();
        
        void makeSound() const;
        std::string getType() const;
};

#endif