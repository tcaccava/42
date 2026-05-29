/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:14:18 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 18:26:15 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FragTrap.hpp"

FragTrap::FragTrap(const std::string& name) : ClapTrap(name)
{
    hitPoints = 100;
    energyPoints = 100;
    attackDamage = 30;
    std::cout << "FragTrap " << name << " has joined the battle!" << std::endl;
}

FragTrap::FragTrap(const FragTrap& other) : ClapTrap(other)
{
    std::cout << "FragTrap " << name << " has been cloned!" << std::endl;
}

FragTrap::~FragTrap()
{
    std::cout << "FragTrap " << name << " has fallen!" << std::endl;
}

FragTrap& FragTrap::operator=(const FragTrap& other)
{
    ClapTrap::operator=(other);
    std::cout << "FragTrap " << name << " has been assigned!" << std::endl;
    return *this;
}

void FragTrap::attack(const std::string& target)
{
    if (energyPoints > 0 && hitPoints > 0)
    {
        std::cout << "FragTrap " << name << " launches a massive attack on " << target 
                  << ", " << attackDamage << " damage!" << std::endl;
        energyPoints--;
    }
    else
    {
        std::cout << "FragTrap " << name << " can't attack!" << std::endl;
    }
}

void FragTrap::highFivesGuys()
{
    std::cout << "FragTrap " << name << " says: High fives, guys!" << std::endl;
}
