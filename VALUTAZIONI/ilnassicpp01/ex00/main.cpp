/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 16:56:08 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/30 16:56:09 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Zombie.hpp"

Zombie* newZombie(std::string name);
void randomChump(std::string name);

/*HEAP zombie (Brad): must be deleted manually, 
otherwise destructor never runs.
STACK zombie (Jill) inside randomChump: destroyed automatically
as soon as randomChump() returns.
STACK zombie inside a scope block (Chris): destroyed automatically
when this block '{ }' ends, BEFORE main() itself returns.
*/
int main()
{
    Zombie* Brad = newZombie("Brad");
    Brad->announce();

    delete Brad;

    randomChump("Jill");

    Zombie chris("Chris");
    chris.announce();

    return 0;
}
