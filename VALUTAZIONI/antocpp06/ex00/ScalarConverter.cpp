/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:31:51 by apintaur          #+#    #+#             */
/*   Updated: 2025/09/04 09:35:02 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <sstream>

static std::string intToString(long long value) {
	std::ostringstream oss;
	oss << value;
	return (oss.str());
}

void ScalarConverter::convert(const std::string& literal) {

	if (literal.size() == 0) {
		displayConversionResult("impossibile", "impossibile", "impossibile",
								"impossibile");
	} else {
		std::string toDisplay = literal;
		char*		endptr;

		if (toDisplay[toDisplay.length() - 1] == 'f')
			toDisplay = toDisplay.substr(0, toDisplay.length() - 1);

		long long rInt = std::strtoll(toDisplay.c_str(), &endptr, 10);
		if (!endptr || endptr == toDisplay.c_str()) {
			rInt = std::numeric_limits<long long>::max();
		}

		std::string decimalBounds = checkDecimalBounds(literal, endptr);

		displayConversionResult(checkCharBounds(rInt), checkIntBounds(rInt),
								decimalBounds + 'f', decimalBounds);
	}
}

void displayConversionResult(const std::string& rChar, const std::string& rInt,
							 const std::string& rFloat,
							 const std::string& rDouble) {

	std::cout << "char: " << rChar << std::endl;
	std::cout << "int: " << rInt << std::endl;
	std::cout << "float: " << rFloat << std::endl;
	std::cout << "double: " << rDouble << std::endl;
}

std::string checkCharBounds(long long rChar) {

	std::string res;

	if ((rChar >= 0 && rChar <= 31) || rChar == 127) {
		res = "Non displayable";
	} else if (rChar < 0 || rChar > 127) {
		res = "impossible";
	} else {
		res = std::string(1, static_cast<char>(rChar));
	}

	return (res);
}

std::string checkIntBounds(long long rInt) {

	std::string res;

	if (rInt > std::numeric_limits<int>::max()
		|| rInt < std::numeric_limits<int>::min()) {
		res = "impossible";
	} else {
		res = intToString(rInt);
	}

	return (res);
}

std::string checkDecimalBounds(const std::string& literal, char* endptr) {

	if (literal == "inf" || literal == "+inf" || literal == "inff"
		|| literal == "+inff") {
		return ("+inf");
	} else if (literal == "-inf" || literal == "-inff") {
		return ("-inf");
	}

	std::string normalizedLiteral;
	long double rDouble;

	normalizedLiteral = literal.substr(0, literal.find_first_of('f'));
	rDouble			  = std::strtold(normalizedLiteral.c_str(), &endptr);

	if (endptr == normalizedLiteral.c_str()) {
		return ("impossible");
	} else if (std::isnan(rDouble)) {
		return ("nan");
	} else if (rDouble > std::numeric_limits<double>::max()
			   || rDouble < std::numeric_limits<double>::min()) {
		return ("impossible");
	}

	if (normalizedLiteral.find('.') == std::string::npos
		&& normalizedLiteral.find('e') == std::string::npos) {
		normalizedLiteral += ".0";
	} else if (normalizedLiteral[normalizedLiteral.length() - 1] == '.') {
		normalizedLiteral += "0";
	}

	return (normalizedLiteral);
}
