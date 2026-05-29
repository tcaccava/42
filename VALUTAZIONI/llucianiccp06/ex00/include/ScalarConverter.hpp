/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:19:47 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/27 16:19:51 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCALARCONVERTER_H
#define SCALARCONVERTER_H

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <cmath>

class ScalarConverter
{
	private:
		ScalarConverter();
		ScalarConverter(const ScalarConverter& other);
		ScalarConverter& operator=(const ScalarConverter& other);
		~ScalarConverter();
	public:
			static void convert(const std::string& literal);
			static bool isChar(const std::string& literal);
			static char convertChar(const std::string& literal);
			static bool edgeCase(const std::string& literal);
			static bool isCharPrint(char c);
			static bool checkValidStr(const std::string& str);
			static bool checkInvalidString(const std::string& literal);

};

#endif
