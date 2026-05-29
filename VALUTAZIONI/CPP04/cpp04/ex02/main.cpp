/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:09:46 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 19:43:16 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AAnimal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include <iostream>

int main() {
    const AAnimal* animals[6];

    // Crea 3 cani e 3 gatti
    for (int i = 0; i < 3; i++) {
        animals[i] = new Dog();
        animals[i+3] = new Cat();
    }

    // Mostra i tipi degli animali e i loro suoni
    for (int i = 0; i < 6; i++) {
        std::cout << animals[i]->getType() << " ";
        animals[i]->makeSound();
    }

    // Elimina gli animali
    for (int i = 0; i < 6; i++) {
        delete animals[i];
    }

    return 0;
}
