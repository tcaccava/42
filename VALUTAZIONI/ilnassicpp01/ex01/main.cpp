/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 16:36:12 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/30 16:36:13 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Zombie.hpp"

Zombie* zombieHorde(int N, std::string name);

int main()
{
    int n = 5;
    Zombie* horde = zombieHorde (n, "Brad");

    for (int i = 0; i < n; i++)
        horde[i].announce();

    delete[] horde;
    std::cout << &horde;
    return (0);
}
