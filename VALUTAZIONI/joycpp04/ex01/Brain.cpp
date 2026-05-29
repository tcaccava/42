/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:34:00 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:05:47 by jcarnebi         ###   ########.fr       */
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

void Brain::setIdea(int index, const std::string& idea) 
{
    if (index >= 0 && index < 100)
        ideas[index] = idea;
}

std::string Brain::getIdea(int index) const 
{
    if (index >= 0 && index < 100)
        return ideas[index];
    return "";
}

