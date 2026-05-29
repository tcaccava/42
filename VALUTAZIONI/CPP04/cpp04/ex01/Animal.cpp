/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:07:35 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 19:36:11 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"

Animal::Animal() : type("Unknown Animal") {
    std::cout << "[Animal] Constructor called\n";
}

Animal::~Animal() {
    std::cout << "[Animal] Destructor called\n";
}

void Animal::makeSound() const {
    std::cout << "[Animal] makes an ambiguous sound\n";
}

std::string Animal::getType() const {
    return type;
}
