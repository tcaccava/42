/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyriarte <jyriarte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:19:29 by jyriarte          #+#    #+#             */
/*   Updated: 2025/06/05 16:20:25 by jyriarte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"

static bool isSpecial(const std::string &param);
static bool isInvalid(const std::string &param);
static Type getType(const std::string &param);
static long double getValue(const std::string &param, Type type);

void ScalarConverter::convert(const std::string &param)
{
    if (isSpecial(param))
        return;
    if (isInvalid(param))
    {
        std::cerr << "invalid conversion" << std::endl;
        return;
    }

    Type type = getType(param);
    long double value = getValue(param, type);

    std::cout << "char: ";
    if (value >= 32 && value <= 126)
        std::cout << "'" << static_cast<char>(value) << "'\n";
    else if (value > CHAR_MAX || value < CHAR_MIN)
        std::cout << "overflow or underflow\n";
    else
        std::cout << "non displayable\n";

    std::cout << "int: ";
    if (value > INT_MAX || value < INT_MIN)
        std::cout << "overflow or underflow\n";
    else
        std::cout << static_cast<int>(value) << "\n";

    bool dotZeroFlag = (static_cast<int>(value) == value && value < 1e6 && (value > 1e-5 || value == 0));

    std::cout << "float: ";
    if (value > FLT_MAX || value < -FLT_MAX)
        std::cout << "overflow or underflow\n";
    else
    {
        std::cout << static_cast<float>(value);
        if (dotZeroFlag)
            std::cout << ".0";
        std::cout << "f\n";
    }

    std::cout << "double: ";
    if (value > DBL_MAX || value < -DBL_MAX)
        std::cout << "overflow or underflow\n";
    else
    {
        std::cout << static_cast<double>(value);
        if (dotZeroFlag)
            std::cout << ".0";
        std::cout << "\n";
    }
}

static Type getType(const std::string &param)
{
    if (param.size() == 3 && param[0] == '\'' && param[2] == '\'')
        return (CHAR);

    if (param.find('.') != std::string::npos)
    {
        if (param[param.size() - 1] == 'f')
            return (FLOAT);
        return (DOUBLE);
    }
    return (INT);
}

static long double getValue(const std::string &param, Type type)
{
    char **end = NULL;
    if (type & CHAR)
        return (param[1]);
    if (type & (INT | FLOAT | DOUBLE))
        return (std::strtold(param.c_str(), end));
    return (0);
}

static bool isSpecial(const std::string &param)
{
    if (param == "nan" || param == "nanf" || param == "+inf" || param == "+inff" || param == "-inf" || param == "-inff")
    {
        std::cout << "char: impossible\n";
        std::cout << "int: impossible\n";
        if (param.size() == 3 || (param.size() == 4 && (param[0] == '-' || param[0] == '+')))
        {
            std::cout << "float: " << param << "f\n";
            std::cout << "double: " << param << std::endl;
        }
        else
        {
            std::cout << "float: " << param << "\n";
            std::cout << "double: " << param.substr(0, param.size() - 1) << std::endl;
        }
        return (true);
    }
    return (false);
}

static bool isInvalid(const std::string &param)
{
    if (param.size() == 0)
        return (true);
    if (param.size() == 3 && param[0] == '\'' && param[2] == '\'')
        return (false);
    bool signFlag = (param[0] == '-' || param[0] == '+');
    bool dotFlag = false;
    for (size_t i = signFlag ? 1 : 0; param[i]; i++)
    {
        if (i != 0 && param[i + 1] && param[i + 1] != 'f' && param[i] == '.')
        {
            if (!dotFlag)
                dotFlag = true;
            else
                return (true);
            continue;
        }
        if (param[i] == 'f' && i == param.size() - 1 && dotFlag)
            continue;
        if (!std::isdigit(param[i]))
            return (true);
    }
    return (false);
}
