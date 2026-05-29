/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:10:03 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/04 19:10:04 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"
#include <string.h>
#include <fstream>

int	main(int argc, char* argv[]) {

	std::ifstream	inputFile;
	std::ofstream	outputFile;
	std::string		filename;
	std::string		s1;
	std::string		s2;

	if (argc != 4)
	{
		std::cout << "Error: program must be started with <filename> <string1> <string2>" << std::endl;
		return (-1);
	}

	filename = argv[1];
	s1 = argv[2];
	s2 = argv[3];

	openFiles(inputFile, outputFile, filename, filename + ".replace");
	copyFile(inputFile, outputFile, s1, s2);
	closeFiles(inputFile, outputFile);
}
