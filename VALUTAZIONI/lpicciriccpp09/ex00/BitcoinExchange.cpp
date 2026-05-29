/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luca <luca@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 17:06:47 by luca              #+#    #+#             */
/*   Updated: 2024/11/13 17:30:10 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"


BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	*this = other;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	(void)other;
	return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

float strToNum(std::string str)
{
	std::stringstream ss(str);
	float result;
	ss >> result;
	return result;
}

std::map<int, float> BitcoinExchange::convert_string(std::string str)
{
	std::string year = str.substr(0, str.find_first_of('-'));
	str = str.substr(str.find_first_of('-') + 1, str.length());
	std::string month = str.substr(0, str.find_first_of('-'));
	str = str.substr(str.find_first_of('-') + 1, str.length());
	std::string day = str.substr(0, str.find_first_of(','));
	std::string data = year + month + day;
	int result = strToNum(data);
	std::string price = str.substr(str.find_first_of(',') + 1, str.length());
	float num = strToNum(price);
	std::map<int, float> input_data;
	input_data[result] = num;
	return input_data;
}

void BitcoinExchange::get_input()
{
	std::ifstream file;
	file.open("data.csv");
	if (!file.is_open())
	{
		std::cerr << "file not found" << std::endl;
		return;
	}
	std::string line;
	std::getline(file, line);
	while(std::getline(file, line))
	{
		if (line.empty())
			continue;
		std::map<int, float> temp_data = convert_string(line);
		input_data.insert(temp_data.begin(), temp_data.end());
		temp_data.clear();
	}
}

int check_line(std::string str)
{
	const char *temp = str.c_str();
	if (*temp != '\0' && std::isspace(*temp))
		temp++;
	while(temp && std::isdigit(*temp))
		temp++;
	if (!temp || *temp != '-')
		return -1;
	temp++;
	if (!temp || !std::isdigit(*temp) || !std::isdigit(*(temp + 1)))
		return -1;
	temp += 2;
	if (!temp || *temp != '-')
		return -1;
	temp++;
	if (!temp || !std::isdigit(*temp) || !std::isdigit(*(temp + 1)))
		return -1;
	temp += 3;
	if (!temp || *temp != '|' || (*(temp + 1)) != 32)
		return -1;
	temp += 2;
	while(*temp != '\0' && std::isdigit(*temp))
		temp++;
	while(*temp != '\0' && std::isspace(*temp) && *temp != '\n')
		temp++;
	if (*temp == '\0' || *temp == '\n')
		return 0;
	temp++;
	if (*temp == '.')
		temp++;
	if (!std::isdigit(*temp))
		return -1;
	while(*temp != '\0' && std::isdigit(*temp))
		temp++;
	while(*temp != '\0' && std::isspace(*temp) && *temp != '\n')
		temp++;
	if (*temp != '\0' && *temp != '\n')
		return -1;
	return 0;
}

void BitcoinExchange::get_exchange(std::string str)
{
	std::string original = str;
	std::string year = str.substr(0, str.find_first_of('-'));
	str = str.substr(str.find_first_of('-') + 1, str.length());
	std::string month = str.substr(0, str.find_first_of('-'));
	str = str.substr(str.find_first_of('-') + 1, str.length());
	std::string day = str.substr(0, str.find_first_of('|'));
	std::string data = year + month + day;
	std::string price = str.substr(str.find_first_of('|') + 1, str.length());
	float result = strToNum(data);
	float num = strToNum(price);
	if (strToNum(month) < 1 || strToNum(month) > 12 || strToNum(day) < 1 || strToNum(day) > 31)
	{
		std::cerr << original << " >> invalid date" << std::endl;
		return ;
	}
	if (num < 0 || num > 1000)
	{
		std::cerr << original << " >> invalid price" << std::endl;
		return ;
	}
	if (result < 20090102 || result > 20220329)
	{
		std::cerr << original << " >> invalid year" << std::endl;
		return ;
	}
	for(std::map<int, float>::iterator it = input_data.begin(); it != input_data.end(); ++it)
	{
		if (it->first == result)
		{
			float final = num * it->second;
			std::cout << original.substr(0, original.find(" |")) << " => " << num << " = " << final << std::endl;
			return ;
		}
		else if (it->first > result)
		{
			--it;
			float final = num * it->second;
			std::cout << original.substr(0, original.find(" |")) << " => " << num << " = " << final << std::endl;
			return ;
		}
	}
	return ;
}

void BitcoinExchange::print_data(char *filename)
{
	std::ifstream file;
	file.open(filename);
	if (!file.is_open())
	{
		std::cerr << "file not found" << std::endl;
		return;
	}
	std::string line;
	std::getline(file, line);
	while(std::getline(file, line))
	{
		if (line.empty())
			continue;
		if (check_line(line) == -1)
		{
			std::cerr << line << " >> invalid input" << std::endl;
			continue;
		}
		else
			get_exchange(line);
	}
	file.close();
}
