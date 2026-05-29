/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:08:03 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 19:36:15 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dog.hpp"

Dog::Dog() {
    type = "Dog";
    std::cout << "[Dog] Constructor called\n";
}

Dog::~Dog() {
    std::cout << "[Dog] Destructor called\n";
}

void Dog::makeSound() const {
    std::cout << "[Dog] Woof!\n";
}
