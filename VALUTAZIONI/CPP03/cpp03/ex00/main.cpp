/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:55:31 by mla-mona          #+#    #+#             */
/*   Updated: 2024/09/18 19:10:47 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

int main() {
    ClapTrap robot("CLAP-01");

    robot.attack("lallero1");
    robot.takeDamage(5);
    robot.beRepaired(3);

    robot.attack("lallero2");
    robot.takeDamage(10);
    robot.beRepaired(4);

    return 0;
}
