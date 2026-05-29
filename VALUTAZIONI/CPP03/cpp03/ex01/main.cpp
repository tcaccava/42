/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:55:31 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 17:30:36 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"
#include "ScavTrap.hpp"

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

    return 0;
}

