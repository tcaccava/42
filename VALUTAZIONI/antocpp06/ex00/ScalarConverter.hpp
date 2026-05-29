/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:31:56 by apintaur          #+#    #+#             */
/*   Updated: 2025/08/13 14:31:57 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCALAR_CONVERTER_HPP
#define SCALAR_CONVERTER_HPP

#include <iostream>

class ScalarConverter {

	private:
		ScalarConverter();
		ScalarConverter(const ScalarConverter&);
		ScalarConverter&	operator=(const ScalarConverter&);
		~ScalarConverter();

	public:
		static void	convert(const std::string& literal);

};

void	displayConversionResult(const std::string& rChar, const std::string& rInt,
								const std::string& rFloat, const std::string& rDouble);
std::string	checkIntBounds(long long rInt);
std::string	checkCharBounds(long long rChar);
std::string	checkDecimalBounds(const std::string& literal, char* endptr);

#endif
