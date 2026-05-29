/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongAnimal.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:09:00 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 19:36:25 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal() : type("WrongAnimal") {
    std::cout << "[WrongAnimal] Constructor called\n";
}

WrongAnimal::~WrongAnimal() {
    std::cout << "[WrongAnimal] Destructor called\n";
}

void WrongAnimal::makeSound() const {
    std::cout << "[WrongAnimal] makes a weird sound!\n";
}

std::string WrongAnimal::getType() const {
    return type;
}
