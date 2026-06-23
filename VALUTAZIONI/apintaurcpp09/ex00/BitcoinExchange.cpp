#include "BitcoinExchange.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <limits>

static std::string	intToStr(long double n) {
	std::ostringstream	oss;
	oss << n;
	return (oss.str());
}

BitcoinExchange::BitcoinExchange() : filename("data.csv") {

	std::ifstream myFile;

	myFile.open(filename.c_str());
	if (!myFile) {
		throw BadFile();
	}

	std::string line;

	if (!std::getline(myFile, line)) {
		throw BadFile();
	}

	try {
		validateHeader(line);
	} catch (const BadData::exception& e) {
		std::cout << "BitcoinExchange: Database 1 - Header: Error: " << e.what()
				  << std::endl;
	}

	while (std::getline(myFile, line)) {
		std::istringstream iss(line);

		std::string date;
		float		value = 0.0f;

		try {
			size_t pos = 0;
			validateDate(line, pos, date);
			validateValue(line, pos, value);
			database[date] = value;
			// std::cout << "BitcoinExchange: Entry loaded: " << date << " | "
			// 		  << value << std::endl;
		} catch (const BadData& e) {
			std::cout << "BitcoinExchange: Database 1: Error: " << e.what()
					  << std::endl;
		}
	}
	myFile.close();
}

BitcoinExchange::BitcoinExchange(const std::string& filename)
	: filename(filename) {
	if (filename.empty()) {
		throw BadFile();
	} else {
		std::ifstream myFile;

		myFile.open(filename.c_str());
		if (!myFile) {
			throw BadFile();
		}

		std::string line;

		if (!std::getline(myFile, line)) {
			throw BadFile();
		}

		try {
			validateHeader(line);
		} catch (const BadData::exception& e) {
			std::cout << "BitcoinExchange: Database 1 - Header: Error: "
					  << e.what() << std::endl;
		}

		while (std::getline(myFile, line)) {
			std::istringstream iss(line);

			std::string date;
			float		value = 0.0f;

			try {
				size_t pos = 0;
				validateDate(line, pos, date);
				validateValue(line, pos, value);
				database[date] = value;
				// std::cout << "BitcoinExchange: Entry loaded: " << date
				// 		  << " | " << value << std::endl;
			} catch (const BadData::exception& e) {
				std::cout << "BitcoinExchange: Database 1: Error: " << e.what()
						  << std::endl;
			}
		}
		myFile.close();
	}
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& b) {
	if (this != &b) {
		database.clear();
		database = b.database;
		filename = b.filename;
	}
	return (*this);
}

float BitcoinExchange::getPriceByDate(const std::string& date) const {
	try {
		std::map<std::string, float>::const_iterator it = database.find(date);
		return (it->second);
	} catch (const std::exception& e) {
		std::cout << "BitcoinExchange: getPriceByDate: Error: " << e.what()
				  << std::endl;
	}
	return (-1);
}

void BitcoinExchange::clear() {
	database.clear();
	filename.clear();
}

void BitcoinExchange::setNewPrice(const std::string& date, const float price) {
	try {
		std::map<std::string, float>::iterator it = database.find(date);
		if (it == database.end()) {
			throw BadData("Date not found: " + date);
		}
		it->second = price;
	} catch (const std::exception& e) {
		std::cout << "BitcoinExchange: getPriceByDate: Error: " << e.what()
				  << std::endl;
	}
}

void BitcoinExchange::load(const std::string& filename) {
	if (filename.empty()) {
		throw BadFile();
	} else {
		std::ifstream myFile;

		myFile.open(filename.c_str());
		if (!myFile) {
			throw BadFile();
		}

		std::string line;

		if (!std::getline(myFile, line)) {
			throw BadFile();
		}

		try {
			validateHeader(line);
		} catch (const BadData::exception& e) {
			std::cout << "BitcoinExchange: Database 2 - Header: Error: "
					  << e.what() << std::endl;
		}

		while (std::getline(myFile, line)) {
			std::istringstream iss(line);

			std::string date;
			float		value = 0.0f;

			try {
				size_t pos = 0;
				validateDate(line, pos, date);
				validateValue(line, pos, value);

				if (value < 0.0f || value > 1000.0f) {
					throw BadData("Value out of range: "
								  + intToStr(value));
				}

				std::map<std::string, float>::iterator it = database.find(date);
				if (it == database.end()) {
					it = database.lower_bound(date);
					if (it == database.begin()) {
						throw BadData(date);
					} else if (it == database.end() || it->first != date) {
						--it;
					}
				}

				float rate	 = it->second;
				float result = value * rate;
				std::cout << date << " => " << value << " = " << result
						  << std::endl;

			} catch (const BadData::exception& e) {
				std::cout << "BitcoinExchange: Database 2: Error: " << e.what()
						  << std::endl;
				continue;
			}
		}
		myFile.close();
	}
}

void BitcoinExchange::print() {
	std::cout << "\n == Database BitcoinExchange ==" << std::endl;
	std::cout << "\n == Format: Date	|	Value	==" << std::endl;
	for (std::map<std::string, float>::iterator i = database.begin();
		 i != database.end(); i++) {
		std::cout << i->first << "	|	" << i->second << std::endl;
	}
}

void BitcoinExchange::validateHeader(std::string& header) {
	trim(header);

	if (header.find("date") == std::string::npos
		|| (header.find("exchange_rate") == std::string::npos
			&& header.find("value") == std::string::npos)) {
		throw BadFile();
	}
	size_t pos = header.find(',');
	if (pos == std::string::npos) {
		pos = header.find('|');
	}
	if (pos == std::string::npos) {
		throw BadFile();
	}
}

void BitcoinExchange::validateDate(const std::string& line, size_t& pos,
								   std::string& date) {
	pos = line.find(',');
	if (pos == std::string::npos) {
		pos = line.find('|');
	}
	if (pos == std::string::npos) {
		throw BadData(line);
	}

	date = line.substr(0, pos);
	trim(date);

	// Valid date: YYYY-MM-DD
	if (date.empty() || date.size() != 10 || date[4] != '-' || date[7] != '-') {
		throw BadData(date);
	}

	for (int i = 0; i < 10; i++) {
		if (i == 4 || i == 7) {
			continue;
		}
		if (!std::isdigit(date[i])) {
			throw BadData(date);
		}
	}

	int year, month, day;

	year  = std::atoi(date.substr(0, 4).c_str());
	month = std::atoi(date.substr(5, 2).c_str());
	day	  = std::atoi(date.substr(8, 2).c_str());

	if (year < 0 || year > 2025 || month < 1 || month > 12 || day < 1
		|| day > 31) {
		throw BadData(date);
	}
}

void BitcoinExchange::validateValue(const std::string& line, size_t pos,
									float& value) {
	std::string valueStr = line.substr(pos + 1);
	trim(valueStr);

	if (valueStr.empty()) {
		throw BadData(valueStr);
	}

	char*		endptr	 = NULL;
	long double tmpValue = std::strtold(valueStr.c_str(), &endptr);

	if (endptr == valueStr.c_str()
		|| tmpValue > std::numeric_limits<float>::max()) {
		throw BadData(valueStr);
	}
	value = static_cast<float>(tmpValue);
}

BitcoinExchange::BadData::BadData(const std::string& l)
	: message("Invalid database data format: " + l) {
}

const char* BitcoinExchange::BadFile::what() const throw() {
	return ("Invalid database file!");
}

const char* BitcoinExchange::BadData::what() const throw() {
	return (message.c_str());
}

BitcoinExchange::BadData::~BadData() throw() {
}

BitcoinExchange::~BitcoinExchange() {
}

// custom trim function for std=c++98 (it was introduced in c++11)
void trim(std::string& s) {
	size_t start = s.find_first_not_of(" \t");
	size_t end	 = s.find_last_not_of(" \t");
	if (start == std::string::npos) {
		s.clear();
		return;
	}
	s = s.substr(start, end - start + 1);
}
