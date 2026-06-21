/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replaceLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:48:42 by ilnassi           #+#    #+#             */
/*   Updated: 2026/06/05 19:45:32 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "replace.hpp"

std::string replaceLine(
	std::string line, 
	const std::string& s1, 
	const std::string& s2)
{
        std::string result;

        while (true)
        {
                size_t pos = line.find(s1);

                if (pos == std::string::npos)
                        break;

                result += line.substr(0, pos);
                result += s2;
                line = line.substr(pos + s1.length());
        }
        result += line;
        return result;
}

