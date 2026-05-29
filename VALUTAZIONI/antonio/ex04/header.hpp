/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:10:16 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/04 19:10:17 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>

void	openFiles(std::ifstream& inputFile, std::ofstream &outputFile,
				const std::string& inputFilename, const std::string& outputFilename);
void	closeFiles(std::ifstream& inputFile, std::ofstream& outputFile);
void	copyFile(std::ifstream& inputFile, std::ofstream& outputFile,
				const std::string& s1, const std::string& s2);
#endif
