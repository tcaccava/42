/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AAnimal.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:07:35 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 20:42:07 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AAnimal.hpp"
#include <iostream>

AAnimal::AAnimal() : type("") {
    std::cout << "[AAnimal] Constructor called\n";
}

AAnimal::~AAnimal() {
    std::cout << "[AAnimal] Constructor called\n";
}

std::string AAnimal::getType() const {
    return type;
}

