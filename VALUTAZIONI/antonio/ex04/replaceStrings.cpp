/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replaceStrings.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:10:01 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/04 20:56:22 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"
#include <string>
#include <iostream>
#include <fstream>

void copyFile(std::ifstream& inputFile, std::ofstream& outputFile,
			  const std::string& s1, const std::string& s2) {

	std::string line;
	std::string result;
	size_t		idx;
	size_t		stringPos;

	while (std::getline(inputFile, line)) {
		idx = 0;
		stringPos = line.find(s1, idx);
		while (stringPos != std::string::npos) {
			result.append(line, idx, stringPos - idx);
			result.append(s2);
			idx = stringPos + s1.length();
			stringPos = line.find(s1, idx);
		}
		result.append(line, idx, line.length() - idx);
		outputFile << result << "\n";
		result.clear();
	}
}
