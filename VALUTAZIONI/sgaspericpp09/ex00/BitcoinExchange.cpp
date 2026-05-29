#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src) {
	if (this != &src) {
		*this = src;
	}
}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange& src) {
	if (this != &src) {
		*this = src;
	}
	return *this;
}

std::map<std::string, double> BitcoinExchange::getInputs() {
	return this->_inputs;
}

void BitcoinExchange::setInputs() {
	this->_inputs = dataParse();
}

void BitcoinExchange::printData(std::string file) {
	std::ifstream inFile;
	inFile.open(file.c_str());
	if (!inFile.is_open())
		throw BitcoinExchange::BadFileFormat();

	std::string line;
	std::getline(inFile, line);
	if (line != "date | value")
		throw BitcoinExchange::BadFileFormat();
	while (std::getline(inFile, line)) {
		std::string date;
		std::string value;
		std::stringstream linestr(line);
		std::getline(linestr, date, '|');
		std::getline(linestr, value, '|');
		// std::cout << "Here date is: " << date << "and value is: " << value << std::endl;
		date = removeSpaces(date);
		if (!isValidDate(date)){
			std::cout << "Error: bad input => " << date << std::endl;
			continue ;
		}
		double numValue = validatePrice(value);
		if (numValue != -1) {
			std::map<std::string, double>::iterator it = _inputs.find(date);
			if (it != _inputs.end()) {
				std::cout << date << " => " << numValue << " = " << numValue * it->second << std::endl;
			}
			else {
				std::map<std::string, double>::iterator itl = _inputs.lower_bound(date);
				if (itl == _inputs.begin())
				std::cout << date << " => " << numValue << " = " << numValue * itl->second << std::endl;
				else {
					--itl;
					std::cout << date << " => " << numValue << " = " << numValue * itl->second << std::endl;
				}
			}
		}
	}
}
