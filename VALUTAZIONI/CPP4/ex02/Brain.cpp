#include "Brain.hpp"
#include <iostream>
#include <string>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Brain::Brain()
	: ideas()
{
	std::cout << "Brain default constructor called." << std::endl;
}

Brain::Brain(const Brain &src)
	: ideas()
{
	std::cout << "Brain copy constructor called." << std::endl;
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Brain::~Brain()
{
	std::cout << "Brain destructor called." << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Brain &Brain::operator=(Brain const &rhs)
{
	if (this != &rhs)
		for (int i = 0; i < 100; i++)
			if (!rhs.ideas[i].empty())
				this->ideas[i].assign(rhs.ideas[i]);
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string Brain::getIdea(int i)
{
	if (i >= 100 || i < 0)
		return std::string();
	return this->ideas[i];
}

void Brain::setIdea(int i, std::string idea)
{
	if (i < 100 && i >= 0)
		this->ideas[i].assign(idea);
}

/* ************************************************************************** */
