/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:14:50 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 18:24:53 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"
#include "ScavTrap.hpp"
#include "FragTrap.hpp"

int main() {
    ClapTrap robot("CLAP-01");
    robot.attack("lallero1");
    robot.takeDamage(5);
    robot.beRepaired(3);
    robot.attack("lallero2");
    robot.takeDamage(10);
    robot.beRepaired(4);

    std::cout << "---------------------" << std::endl;

    ScavTrap gateKeeper("SCAV-99");
    gateKeeper.attack("thief");
    gateKeeper.takeDamage(20);
    gateKeeper.beRepaired(15);
    gateKeeper.guardGate();

    std::cout << "---------------------" << std::endl;

    FragTrap highFiver("FRAG-X");
    highFiver.attack("enemy");
    highFiver.takeDamage(50);
    highFiver.beRepaired(25);
    highFiver.highFivesGuys();

    return 0;
}
