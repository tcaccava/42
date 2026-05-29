/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:08:03 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 20:46:58 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dog.hpp"
#include <iostream>

Dog::Dog() {
    type = "Dog";
    brain = new Brain();
    std::cout << "Dog constructor\n";
}

Dog::Dog(const Dog &other) {
    type = other.type;
    brain = new Brain(*other.brain); // deep copia
    std::cout << "Dog copy constructor\n";
}

Dog &Dog::operator=(const Dog &other) {
    if (this != &other) {
        delete brain; // Elimina la memoria precedente
        brain = new Brain(*other.brain); // deep copia
        type = other.type;
    }
    std::cout << "Dog assignment operator\n";
    return *this;
}

Dog::~Dog() {
    delete brain;
    std::cout << "Dog destructor\n";
}

void Dog::makeSound() const {
    std::cout << "Woof!" << std::endl;
}
