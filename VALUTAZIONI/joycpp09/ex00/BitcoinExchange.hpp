#ifndef BitcoinExchange_HPP
#define BitcoinExchange_HPP

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <map>
#include <sstream>
#include <limits.h>

class BitcoinExchange
{
	private:
		std::string data_filename;
		std::string input_filename;
		BitcoinExchange();
	public:
		BitcoinExchange(std::string filename);
		~BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		void run();
};

enum e_date
{
	year,
	month,
	day
};
#endif