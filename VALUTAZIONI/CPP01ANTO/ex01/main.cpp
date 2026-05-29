/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:12:34 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/09 09:16:04 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

const std::string	zombieName = "Guglielmo";
const int			N = 10;

int	main( void ) {
	Zombie*		zombies;

	zombies = zombieHorde(N, zombieName);

	if (zombies) {
		for (int i = 0; i < N; i++) {
			if (zombies[i].getName() != zombieName) {
				std::cout << "Something gone wrong within the zombieHorde function!" << std::endl;
				break ;
			}
			zombies[i].announce();
		}
		delete[] zombies;
	}
	return (0);
}
