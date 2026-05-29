/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:08:31 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 20:47:08 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"
#include <iostream>

Cat::Cat() {
    type = "Cat";
    brain = new Brain();
    std::cout << "Cat constructor\n";
}

Cat::Cat(const Cat &other) {
    type = other.type;
    brain = new Brain(*other.brain); // deep copia
    std::cout << "Cat copy constructor\n";
}

Cat &Cat::operator=(const Cat &other) {
    if (this != &other) {
        delete brain; // Elimina la memoria precedente
        brain = new Brain(*other.brain); // deep copia
        type = other.type;
    }
    std::cout << "Cat assignment operator\n";
    return *this;
}

Cat::~Cat() {
    delete brain;
    std::cout << "Cat destructor\n";
}

void Cat::makeSound() const {
    std::cout << "Meow!" << std::endl;
}
