/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongCat.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:35:01 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:16:50 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRONGCAT_HPP
#define WRONGCAT_HPP

#include "WrongAnimal.hpp"
#include <iostream>

class WrongCat: public WrongAnimal 
{
    public:
        WrongCat();
        WrongCat(const WrongCat &instance);
        WrongCat& operator=(const WrongCat &instance);
        ~WrongCat();
        
        void makeSound() const;
        std::string getType() const;
};

#endif