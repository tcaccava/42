/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:49:25 by ilnassi           #+#    #+#             */
/*   Updated: 2026/06/05 19:46:28 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "replace.hpp"

int     main(int ac, char *av[])
{
        if (ac != 4)
        {
                std::cerr << "Usage: ./replace <filename> <s1> <s2>" << std::endl;
                return 1;
        }

        const std::string filename = av[1];
        const std::string s1       = av[2];
        const std::string s2       = av[3];

        if (s1.empty())
        {
                std::cerr << "Error: s1 non puo' esser vuota" << std::endl;
                return 1;
        }

        std::ifstream infile(filename.c_str());
        if (!infile.is_open())
        {
                std::cerr << "Error: File non trovato!" << std::endl;
                return 1;
        }

        std::ofstream outfile((filename + ".replace").c_str());
        if (!outfile.is_open())
        {
                std::cerr << "Error: impossibile creare il file!" << std::endl;
                return 1;
        }

        std::string line;
        while (std::getline(infile, line))
        {
                outfile << replaceLine(line, s1, s2) << "\n";
        }

        infile.close();
        outfile.close();
        return (0);
}

