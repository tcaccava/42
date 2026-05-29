#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <stdexcept>


BitcoinExchange::BitcoinExchange(std::string input_filename)
{
	this->data_filename = "data.csv";
	this->input_filename = input_filename;
}
BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	//*this = other;
	this->data_filename = other.data_filename;
    this->input_filename = other.input_filename;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if(this != &other)
	{
		this->data_filename = other.data_filename;
		this->input_filename = other.input_filename;
	}
	return *this;
}

std::string& rtrim(std::string& s)
{
	const char* t = " \t\n\r\f\v";
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

std::string& ltrim(std::string& s)
{
	const char* t = " \t\n\r\f\v";
	s.erase(0, s.find_first_not_of(t));
	return s;
}

std::string& trim(std::string& s)
{
	return ltrim(rtrim(s));
}

bool is_valid_date(std::map<int,int> date)
{
	//if (date[year] > 2023 || date[year] < 2009) // to change
		//return false; 
	if (date[month] < 1 || date[month] > 12)
		return false; 
	if (date[day] < 1 || date[day] > 31) 
		return false; 
  	if(date[year] == 2009 && date[month] == 1 && date[day] < 2)
		return false;
	
	if (date[month] == 2) 
	{ 
		if (date[year] % 4 == 0) 
			return (date[day] <= 29); 
		else
			return (date[day] <= 28);
	} 

	// April 4, June 6, Sept 9, Nov 11 - 30 days
	if (date[month] == 4 || date[month] == 6 || date[month] == 9 || date[month] == 11) 
		return (date[day] <= 30);
	return true;
}
bool check_key_validity(std::string key, int i)
{
	if(key.empty())
		return false;
	if(!i && key != "date")
	{
		std::cout << "Error: Not a valid header => " << key << std::endl;
		return false;
	}
	if(i && std::count(key.begin(), key.end(), '-') != 2)
		return false;
	if(i && key.find_first_not_of("0123456789-") != std::string::npos)
	{
		std::cout << "Error: Not a Number => " << key << std::endl;
		return false;
	}
	if(i && (key.find('-') == key.length() - 1 || key.find('-') == 0))
	{
		std::cout << "Error: Not Right Number of dashes" << std::endl;
		return false;
	}
	if(i && key.length() != 10)
	{
		std::cout << "Error: Not a Valid Date => " << key << std::endl;
		return false;
	}
	if(i && (key.find_first_of('-') != 4 || key.find_last_of('-') != 7))
	{
		std::cout << "Error: Not Right Number of dashes" << std::endl;
		return false;
	}
	if(i && (key.find_first_of('-') == 0 || key.find_last_of('-') == key.length() - 1))
	{
		std::cout << "Error: Not Right Number of dashes" << std::endl;
		return false;
	}
	

	std::stringstream ss(key);
	std::string token;
	std::map<int,int> date;
	int count = 0;
	while(std::getline(ss, token, '-'))
		date[count++] = atoi(token.c_str());


	if(i && is_valid_date(date) == 0)
	{
		std::cout << "Error: Not a Valid Date => " << key << std::endl;
		return false;
	}
	return true;
}

bool check_value_validity(std::string value, int i, int x)
{
	if(value.empty())
	{
		std::cout << "Error: Empty Value => " << value << std::endl;
		return false;
	}
	if(!i && !x && value != "exchange_rate")
	{
		std::cout << "Error: Invalid Header => " << value << std::endl;
		return false;
	}
	else if(!i && x && value != "value")
	{
		std::cout << "Error: Invalid Header => " << value << std::endl;
		return false;
	}
	if(i && value.find_first_not_of("0123456789.") != std::string::npos)
	{
		std::cout << "Error: Not a valid Number => " << value << std::endl;
		return false;
	}
	if(i && std::count(value.begin(), value.end(), '.') > 1)
	{
		std::cout << "Error: Not a valid Number => " << value << std::endl;
		return false;
	}
	if(i && (value.find('.') == value.length() - 1 || value.find('.') == 0))
	{
		std::cout << "Error: Not a valid Number => " << value << std::endl;
		return false;
	}
	if(i && x && atof(value.c_str()) > 1000)
	{
		std::cout << "Error: Number Too big => " << value << std::endl;
		return false;
	}
	if(i && (value.length() > 10 || atof(value.c_str()) > INT_MAX))
	{
		std::cout << "Error: Number Too big => " << value << std::endl;
		return false;
	}
	return true;
}
int convert_to_date(std::string key)
{
	std::stringstream ss(key);
	std::string token;
	std::map<int,int> date;
	int count = 0;
	while(std::getline(ss, token, '-'))
		date[count++] = atoi(token.c_str());
	return date[year] * 10000 + date[month] * 100 + date[day];
}

std::map<int, float> read_and_parse_data_file(std::string filename)
{
	std::map<int, float> map;
	std::fstream file;
	std::string line ;
	int i = 0;

	try {
		file.open(filename.c_str(), std::ios::in);
		if (!file.is_open())
			throw std::exception();
		while (std::getline(file, line)) {
			int seperator = line.find(',');
			int count = std::count(line.begin(), line.end(), ',');
			if(count != 1 || seperator == -1)
				throw std::exception();
			std::string key = line.substr(0, seperator);
			std::string value = line.substr(seperator + 1, line.length());

			if(!check_key_validity(key, i) || !check_value_validity(value, i, 0))
				throw std::exception();

			int date = convert_to_date(key);
			float exchange_rate = atof(value.c_str());
			map[date] = exchange_rate;
			i++;
		}
	}
	catch (std::exception &e) { std::cout << "Error: Bad Data file" << std::endl; exit(1);}
	return map;
}

void find_nearest_date(std::map<int, float> data_map, int date, std::string key, std::string value)
{
	std::map<int, float>::iterator it = data_map.lower_bound(date);

		if (it == data_map.begin())
		{
			std::cout << "Error: no earlier date available" << std::endl;
			return;
		}
--it;
std::cout << key << " => " << it->second * atof(value.c_str()) << std::endl;
}

void parse_input_file(std::string filename, std::map<int, float> data_map)
{
	std::fstream file;
	std::string line ;
	int i = 0;

	try {
		file.open(filename.c_str(), std::ios::in);
		if (!file.is_open())
			throw std::exception();
		if(file.peek() == std::ifstream::traits_type::eof())
			throw std::exception();
		while (std::getline(file, line)) {
			if(line.empty())
			{
				std::cout << "Error: bad input => " << line << std::endl;
				i++;
				continue;
			}
			int seperator = line.find('|');
			//int seperator = line.find(',');
			int count = std::count(line.begin(), line.end(), '|');
			//int count = std::count(line.begin(), line.end(), ',');
			if(count != 1 || seperator == -1)
			{
				std::cout << "Error: bad input => " << line << std::endl;
				i++;
				continue;
			}
			/*if(std::count(line.begin(), line.end(), ' ') != 2)
			{
				std::cout << "Error: bad input => " << line << std::endl;
				i++;
				continue;
			}*/
			std::string key = line.substr(0, seperator);
			key = trim(key);
			std::string value = line.substr(seperator + 1, line.length());
			value = trim(value);

			int date = convert_to_date(key);
			
			if(!check_key_validity(key, i) || !check_value_validity(value, i, 1))
			{
				i++;
				continue;
			}
			
			if(i)
			{
				if(data_map.find(date) == data_map.end())
					find_nearest_date(data_map, date, key, value);
				else
					std::cout << key << " => " << data_map[date] * atof(value.c_str()) << std::endl;
			}

			i++;
		}
	}
	catch (std::exception &e) { std::cout << "Error: Bad input file" << std::endl; exit(1);}
}

void BitcoinExchange::run()
{
	std::map<int, float> data_map = read_and_parse_data_file(data_filename);
	parse_input_file(input_filename, data_map);
}