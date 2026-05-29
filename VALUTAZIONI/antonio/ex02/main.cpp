/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:01:19 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/04 15:01:20 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int	main( void ) {
	std::string		mainString = "HI THIS IS BRAIN";
	std::string*	stringPTR = &mainString;
	std::string&	stringREF = mainString;

	std::cout << "Memory address of the string variable:	[" << &mainString << "]" << std::endl;
	std::cout << "Memory address held by stringPTR:	[" << stringPTR << "]" << std::endl;
	std::cout << "Memory address held by stringREF: 	[" << &stringREF << "]" << std::endl;
	std::cout << std::endl;

	std::cout << "Value of the string variable:		[" << mainString << "]" << std::endl;
	std::cout << "Value pointed to by stringPTR:		[" << *stringPTR << "]" << std::endl;
	std::cout << "Value pointed to by stringREF:		[" << stringREF << "]" << std::endl;
}
