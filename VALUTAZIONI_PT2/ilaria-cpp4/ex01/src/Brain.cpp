/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 00:52:53 by ilnassi           #+#    #+#             */
/*   Updated: 2026/11/19 12:59:27 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Brain.hpp"

Brain::Brain()
{
	std::cout << "Brain constructor called" << std::endl;
}

Brain::Brain(const Brain& other)
{
	std::cout << "Brain copy constructor called" << std::endl;
	*this = other;
}

/*Copy every single element of the `ideas` array from the original brain (`other`) 
into the current brain (`*this`), ensuring that the two objects contain identical 
but completely independent data.*/
Brain& Brain::operator=(const Brain& other)
{
	std::cout << "Brain operator called" << std::endl;
	if (this != &other)
	{
		for (int i = 0; i < 100; i++)
		{
			/*It takes the string at position i from `other` (the source) 
			ad copies it to position i in `this` (the destination). 
			Since it is an `std::string`, 
			it creates an actual copy of the text in memory.*/
			this->ideas[i] = other.ideas[i];
		}
	}
	return *this;
}

Brain::~Brain()
{
	std::cout << "Brain destructor called" << std::endl;
}

/*Verify that the requested index falls within the valid limits of the array (from 0 to 99)
Returns the corresponding string; otherwise, it returns an empty string*/
std::string Brain::getIdea(int idx) const
{
	if (idx >= 0 && idx < 100)
		return this->ideas[idx];
	return "";
}

/*Pass the string by constant reference. This avoids creating an unnecessary copy of
 the string in memory every time you call the function, making the code faster 
 and more efficient. */
void	Brain::setIdea(int idx, const std::string& idea)
{
	if (idx >= 0 && idx < 100)
		this->ideas[idx] = idea;
}
