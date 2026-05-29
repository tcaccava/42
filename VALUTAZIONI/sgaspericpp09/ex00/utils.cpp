#include "BitcoinExchange.hpp"
#include <cctype>
#include <sstream>

double ft_atof(std::string n, std::string date) {
	double i = 0;
	if (n.find("-") != n.npos) {
		std::cout << "Error: not a positive number: " << n << std::endl;
		return (-1);
	}
	if (n.find("null") != n.npos) {
		std::cout << "Error: bad input => " << n << date << std::endl;
		return (-1);
	}
	std::stringstream ss;
	ss << n;
	if (!(ss >> i)) {
		std::cout << "Error: bad input => " << n << date << std::endl;
		return (-1);
	}
	return (i);
}

bool isValidDate(std::string date) {
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	for (int i = 0; i < 10; i++) {
		if (i == 4 || i == 7)
			continue ;
		if (isdigit(date[i]) == 0)
			return (false);
	}
	if (date[5] == '0' && date[6] == '0')
		return (false);
	if ((date[5] == '1' && date[6] > '2') || date[5] > '1')
		return (false);

	if (date[8] == '0' && date[9] == '0')
		return (false);
	if ((date[8] == '3' && date[9] > '1') || date[8] > '3')
		return (false);
	return (true);
}

std::map<std::string, double> dataParse() {
    std::ifstream inFile;
	inFile.open("data.csv");
	if (!inFile) {
		std::cout << "Error: bad file." << std::endl;
	}
    std::string line;
	std::getline(inFile, line);
	std::string dividers[2] = {",", "|"};
	std::map<std::string, double> returnMap;
	while (std::getline(inFile, line)) {
		std::string divider = line.find(dividers[0]) != line.npos ? dividers[0] : dividers[1];
		std::string key = removeSpaces(line.substr(0, line.find(divider)));
		std::string value = line.substr(line.find(divider) + 1);
		returnMap.insert(std::pair<std::string, double>(key, ft_atof(value, key)));
	}
    inFile.close();
    return (returnMap);
}

double validatePrice(std::string const &priceStr) {
	double returnPrice;

	std::stringstream streamValue(priceStr);
	size_t firstMinus = priceStr.find("-");
	size_t lastMinus = priceStr.rfind("-");
	bool multipleMinus = (firstMinus != std::string::npos && firstMinus != lastMinus);
	if(!(streamValue >> returnPrice) || multipleMinus) {
		std::cout << "Error: Bad value input." << std::endl;
		return (-1);
	}
	else if (returnPrice < 0) {
		// std::cout << "Here return price is: " << returnPrice << std::endl;
		std::cout << "Error: Negative number not allowed." << std::endl;
		return (-1);
	}
	else if (returnPrice > 1000) {
		std::cout << "Error: too large a number." << std::endl;
		return (-1);
	}
	return (returnPrice);
}

std::string removeSpaces(std::string div) {
	for (int i = div.length(); i >= 0; i--) {
		if (div.find(' ') != div.npos) 
			div.erase(div.find(' '), 1);
	}
	return (div);
}
