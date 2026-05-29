/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luca <luca@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 17:06:50 by luca              #+#    #+#             */
/*   Updated: 2024/11/12 19:31:46 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <iomanip>
#include <map>
#include <cstring>
#include <fstream>
#include <sstream>

#define DATE_ERROR -7
#define PRICE_ERROR -8

class BitcoinExchange
{
	public:
		std::map<int, float> input_data;
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();
		void get_input();
		std::map<int, float> convert_string(std::string str);
		void get_exchange(std::string str);
		void print_data(char *filename);
};

#endif

