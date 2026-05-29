/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:18:19 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/27 16:18:22 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"

ScalarConverter::ScalarConverter()
{

}

ScalarConverter::ScalarConverter(const ScalarConverter& other)
{
	(void)other;
}

ScalarConverter& ScalarConverter::operator=(const ScalarConverter& other)
{
	if(this != &other)
	{

	}
	return *this;
}

ScalarConverter::~ScalarConverter()
{

}

bool ScalarConverter::isChar(const std::string& literal)
{
	if(literal.size() == 1 && (!std::isdigit(literal[0])))
		return true;
	if(literal[0] == '\'' && literal[literal.size() - 1] == '\'')
		return true;
	return false;
}

bool ScalarConverter::checkValidStr(const std::string& str)
{
	int point = 0;
	int op = 0;
	int f = 0;

	if(str.empty())
		return false;
	for(size_t i = 0; i < str.size(); i++)
	{
		if(str[i] == '.')
			point++;
		else if(str[i] == '+' || str[i] == '-')
			op++;
		else if(str[i] == 'f')
			f++;
	}
	if(op == 1 && (str[0] != '+' && str[0] != '-'))
		return false;
	if(f == 1 && (str[str.size() - 1] != 'f'))
		return false;
	if(point > 1 || op > 1 || f > 1)
		return false;
	if(str.size() == 1 && str[0] == ' ')
		return false;
	
	return true;
}

bool ScalarConverter::checkInvalidString(const std::string& literal)
{

	size_t i = 0;
	size_t validChar = 0;
	while(i < literal.size())
	{
		if((literal[i] >= 'a' && literal[i] <= 'e') || (literal[i] >= 'g' && literal[i] <= 'z') || (literal[i] >= 'A' && literal[i] <= 'Z'))
			validChar++;
		i++;
	}
	if(literal.size() > 1 && validChar >= 1)
		return false;
	if(literal[0] == 'f' && literal.size() > 1)
		return false;
	return true;
}

char ScalarConverter::convertChar(const std::string& literal)
{
	std::istringstream iss(literal);
	char c ;
	iss >> c;
	return c;
}

bool ScalarConverter::edgeCase(const std::string& literal)
{
	if(literal == "nan" || literal == "+inf" || literal == "-inf" || literal == "nanf" || literal == "+inff" || literal == "-inff")
	{
		std::cout << "char: impossible" << std::endl;
		std::cout << "int: impossible" << std::endl;
		if(literal == "+inff" || literal == "-inff")
		{
			std::cout << "float: " << ((literal == "+inff") ? "+inff" : "-inff") << std::endl;
			std::cout << "double: " << ((literal == "+inff") ? "+inf" : "-inf") << std::endl;
		}
		else if(literal == "+inf" || literal == "-inf")
		{
			std::cout << "float: " << ((literal == "+inf") ? "+inff" : "-inff") << std::endl;
			std::cout << "double: " << ((literal == "+inf") ? "+inf" : "-inf") << std::endl;
		}
		else if(literal == "nanf" || literal == "nan")
		{
			std::cout << "float: " << ((literal == "nanf") ? "nanf" : "nanf") << std::endl;
			std::cout << "double: " << ((literal == "nan") ? "nan" : "nan") << std::endl;
		}
		return true;
	}
	return false;
}

bool ScalarConverter::isCharPrint(char c)
{
	if(c < 32 || c > 126 || c == 0)
		return false;
	return true;
}


void ScalarConverter::convert(const std::string& literal)
{
	if (edgeCase(literal) == true)
		return;
	if(checkInvalidString(literal) == false)
	{
		std::cout << "Error: invalid literal" << std::endl;
		return;
	}
	if(checkValidStr(literal) == false)
	{
		std::cout << "Error: invalid literal" << std::endl;
		return;
	}
	if(isChar(literal))
	{
		char c = convertChar(literal);
		int i = static_cast<int>(c);
		double d = static_cast<double>(i);
		float f = static_cast<float>(i);
		if(isCharPrint(c) == false) //test
			std::cout << "char: " << "Non displayable" << std::endl;
		else
			std::cout << "char: " << "\'" << c << "\'" << std::endl;
		std::cout << "int: " << i << std::endl;
		std::cout << "float: " << std::fixed << std::setprecision(1) << f << "f" << std::endl;
		std::cout << "double: " << std::fixed << std::setprecision(1) << d << std::endl;
	}
	else
	{
		double d = std::strtod(literal.c_str(), NULL);
		if(d > std::numeric_limits<int>::max() || d < std::numeric_limits<int>::min())
		{	
			std::cout << "char: " << "impossible" << std::endl;	
			std::cout << "int: " << "impossible" << std::endl;
		}
		else
		{
			if (d < 0 ||  d > 127 || std::isnan(d))
				std::cout << "char: " << "impossible" << std::endl;
			else
			{
				char c = static_cast<char>(d);
				if(isCharPrint(c) == false)
					std::cout << "char: " << "Non displayable" << std::endl;
				else
					std::cout << "char: " << "\'" << c << "\'" << std::endl;
			}
			int i  = static_cast<int>(d);
			std::cout << "int: " << i << std::endl;
		}
		float f = static_cast<float>(d);
		std::cout << "float: " << std::fixed << std::setprecision(1) << f << "f" << std::endl;
		std::cout << "double: " << std::fixed << std::setprecision(1) << d << std::endl;
	}
	return;
} 
