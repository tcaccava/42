/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 10:41:23 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/05 11:05:25 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Harl.hpp"

int	main(int argc, char *argv[]) {
	Harl		harl;

	if (argc <= 1) {
		std::cout << "[  Probably complaining about insignificant problems ]" << std::endl;
		return (0);
	}
	harl.printLevels(argv[1]);
}
