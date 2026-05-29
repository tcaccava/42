/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:10:12 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/04 19:10:13 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"
#include <fstream>

void	openFiles(std::ifstream& inputFile, std::ofstream &outputFile,
				const std::string& inputFilename, const std::string& outputFilename) {

	inputFile.open(inputFilename, std::ios::in);

	if (!inputFile.is_open()) {
		std::cerr << "Error: something wrong with the provided input file!" << std::endl;
		exit (EXIT_FAILURE);
	}

	outputFile.open(outputFilename, std::ios::out);

	if (!outputFile.is_open()) {
		std::cerr << "Error: something wrong with the provided output file!" << std::endl;
		inputFile.close();
		exit (EXIT_FAILURE);
	}

}

void	closeFiles(std::ifstream& inputFile, std::ofstream& outputFile) {
	inputFile.close();
	outputFile.close();
}

