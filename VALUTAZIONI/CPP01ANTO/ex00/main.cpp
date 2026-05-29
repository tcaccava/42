/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:12:51 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/04 14:12:52 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

int	main( void ) {
	Zombie	stackZombie("Pietro");
	Zombie	*heapZombie;

	stackZombie.announce();
	heapZombie = newZombie("Gennaro");
	heapZombie->announce();
	delete heapZombie;
	randomChump("Giliberto");

	return (0);
}
