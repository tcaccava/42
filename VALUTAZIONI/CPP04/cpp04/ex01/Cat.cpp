/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:08:31 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 20:34:55 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"

Cat::Cat() {
    type = "Cat";
    std::cout << "[Cat] Constructor called\n";
}

Cat::~Cat() {
    std::cout << "[Cat] Destructor called\n";
}

void Cat::makeSound() const {
    std::cout << "[Cat] Meow!\n";
}
