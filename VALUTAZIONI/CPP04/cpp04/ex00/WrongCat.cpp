/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongCat.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:09:34 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 19:36:03 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WrongCat.hpp"

WrongCat::WrongCat() {
    type = "WrongCat";
    std::cout << "[WrongCat] Constructor called\n";
}

WrongCat::~WrongCat() {
    std::cout << "[WrongCat] Destructor called\n";
}

void WrongCat::makeSound() const {
    std::cout << "[WrongCat] Meow...? (but won't override if called through pointer)\n";
}
