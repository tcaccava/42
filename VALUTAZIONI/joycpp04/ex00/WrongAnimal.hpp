/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongAnimal.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:33:14 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:15:21 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRONGANIMAL_HPP
#define WRONGANIMAL_HPP

#include <iostream>

class WrongAnimal 
{
    protected:
        std::string type;
    public:
        WrongAnimal();
        WrongAnimal(std::string Type);
        WrongAnimal(const WrongAnimal &instance);
        WrongAnimal& operator=(const WrongAnimal &instance);
        ~WrongAnimal();

        void makeSound() const;
        std::string getType() const;
};

#endif