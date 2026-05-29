/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:56:39 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 21:33:54 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Brain.hpp"

Brain::Brain( void ) : head(0), tail(0), size(0) {
	std::cout << "Brain default constructor called" << std::endl;
	for (int i = 0; i < MAX_IDEAS; i++) {
		ideas[i] = "(null)";
	}
}

Brain::Brain( const std::string& idea ) : head(0), tail(1), size(1) {
	std::cout << "Brain constructor with parameters called" << std::endl;
	ideas[0] = idea;
	for (int i = 1; i < MAX_IDEAS; i++) {
		ideas[i] = "(null)";
	}
}

Brain::Brain( const Brain& brain ) {
	std::cout << "Brain copy constructor called" << std::endl;
	for (int i = 0; i < MAX_IDEAS; i++) {
		ideas[i] = brain.ideas[i];
	}
	head = brain.head;
	tail = brain.tail;
	size = brain.size;
}

Brain&	Brain::operator=( const Brain& brain ) {
	std::cout << "Brain copy assignment constructor called" << std::endl;
	if (this != &brain) {
		for (int i = 0; i < MAX_IDEAS; i++) {
			ideas[i] = brain.ideas[i];
		}
		head = brain.head;
		tail = brain.tail;
		size = brain.size;
	}
	return (*this);
}

void	Brain::setIdea( const std::string& idea ) {
	ideas[tail] = idea;

	if (isFull()) {
		head = (head + 1) % MAX_IDEAS;
	} else {
		size++;
	}

	tail = (tail + 1) % MAX_IDEAS;
}

const std::string&	Brain::getIdea( const int& idx ) const {
	if (idx < 0 || idx >= size) {
		static const std::string nullStr = "(null)";
		return nullStr;
	}
	return (ideas[(head + idx) % MAX_IDEAS]);
}

Brain::~Brain( void ) {
	std::cout << "Brain destructor called" << std::endl;
}
