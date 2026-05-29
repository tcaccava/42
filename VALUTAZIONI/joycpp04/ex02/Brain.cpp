/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:35:33 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/30 15:48:22 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Brain.hpp"

Brain::Brain()
{
    std::cout << "Brain constructor called" << std::endl;
}


Brain::Brain(const Brain &obj)
{
    std::cout << "Brain Copy constructor called" << std::endl;
    for (int i = 0; i < 100; ++i)
        this->ideas[i] = obj.ideas[i];
}

Brain& Brain::operator=(const Brain &obj)
{
    std::cout << "Brain Copy assignment operator called" << std::endl;
    if (this != &obj)
    {
        for(int i=0; i< 100; i++)
            this->ideas[i] = obj.ideas[i];
    }
    return *this;
}

Brain::~Brain()
{
    std::cout << "Brain Destructor called" << std::endl;
}


