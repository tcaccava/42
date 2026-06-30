#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _db(other._db) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
		_db = other._db;
	return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

// Removes leading/trailing spaces and tabs from a string.
static std::string	trim(const std::string& s)
{
	std::string::size_type	start = s.find_first_not_of(" \t");
	std::string::size_type	end = s.find_last_not_of(" \t");
	if (start == std::string::npos)
		return ("");
	return (s.substr(start, end - start + 1));
}

void	BitcoinExchange::loadDatabase(const std::string& filename)
{
	std::ifstream	file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("could not open database file.");

	std::string	line;
	std::getline(file, line); // skip the header "date,exchange_rate"
	while (std::getline(file, line))
	{
		std::string::size_type	comma = line.find(',');
		if (comma == std::string::npos)
			continue ;
		std::string	date = trim(line.substr(0, comma));
		std::string	rateStr = trim(line.substr(comma + 1));
		if (date.empty() || rateStr.empty())
			continue ;
		double	rate = std::atof(rateStr.c_str());
		_db[date] = rate;
	}
	if (_db.empty())
		throw std::runtime_error("database is empty.");
}

bool	BitcoinExchange::isValidDate(const std::string& date) const
{
	// Expected format: YYYY-MM-DD (10 chars, dashes at positions 4 and 7)
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	for (std::string::size_type i = 0; i < date.size(); ++i)
	{
		if (i == 4 || i == 7)
			continue ;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return (false);
	}
	int	year = std::atoi(date.substr(0, 4).c_str());
	int	month = std::atoi(date.substr(5, 2).c_str());
	int	day = std::atoi(date.substr(8, 2).c_str());
	if (month < 1 || month > 12 || day < 1 || day > 31)
		return (false);
	int	mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	// Leap year -> February has 29 days
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		mdays[1] = 29;
	if (day > mdays[month - 1])
		return (false);
	return (true);
}

double	BitcoinExchange::getRate(const std::string& date) const
{
	// lower_bound: first element with key >= date
	std::map<std::string, double>::const_iterator	it = _db.lower_bound(date);
	if (it != _db.end() && it->first == date)
		return (it->second); // exact date found
	if (it == _db.begin())
		throw std::runtime_error("no earlier date in database.");
	--it; // take the closest lower date
	return (it->second);
}

void	BitcoinExchange::evaluateLine(const std::string& line) const
{
	std::string::size_type	bar = line.find('|');
	if (bar == std::string::npos)
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return ;
	}
	std::string	date = trim(line.substr(0, bar));
	std::string	valueStr = trim(line.substr(bar + 1));

	if (!isValidDate(date))
	{
		std::cout << "Error: bad input => " << date << std::endl;
		return ;
	}
	if (valueStr.empty())
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return ;
	}
	// Manual numeric validation (single dot, digits and sign only)
	char*	endptr = 0;
	double	value = std::strtod(valueStr.c_str(), &endptr);
	if (*endptr != '\0')
	{
		std::cout << "Error: bad input => " << valueStr << std::endl;
		return ;
	}
	if (value < 0)
	{
		std::cout << "Error: not a positive number." << std::endl;
		return ;
	}
	if (value > 1000)
	{
		std::cout << "Error: too large a number." << std::endl;
		return ;
	}
	try
	{
		double	rate = getRate(date);
		std::cout << date << " => " << value << " = " << (value * rate) << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}

void	BitcoinExchange::processInput(const std::string& filename)
{
	std::ifstream	file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("could not open file.");

	std::string	line;
	std::getline(file, line); // skip the header "date | value"
	if (trim(line) != "date | value")
		evaluateLine(line); // if it was not a header, evaluate the line anyway
	while (std::getline(file, line))
	{
		if (trim(line).empty())
			continue ;
		evaluateLine(line);
	}
}
