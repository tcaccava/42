/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:31:45 by apintaur          #+#    #+#             */
/*   Updated: 2025/08/13 14:31:46 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"


int	main(int argc, char* argv[]) {

	if (argc != 2) {
		return (-1);
	}

	ScalarConverter::convert(argv[1]);

	return (0);
}
