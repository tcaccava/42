#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : database(other.database) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
        database = other.database;
    return *this;
}

bool BitcoinExchange::create_database(const std::string &csv)
{
    std::ifstream file(csv.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error opening csv file" << std::endl;
        return false;
    }
    std::string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;
        if (line.find("date") != std::string::npos)
            continue;
        std::string::size_type comma = line.find(",");
        std::string date = line.substr(0, comma);
        std::string value = line.substr(comma + 1);
        float fvalue = strtof(value.c_str(), NULL);
        database[date] = fvalue;
    }
    return !database.empty();
}

bool BitcoinExchange::is_valid_date(const std::string date)
{
    if (date.length() != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (!isdigit(date[i]))
            return false;
    }

    char* endptr;
    long year = strtol(date.substr(0, 4).c_str(), &endptr, 10);
    if (*endptr != '\0') return false;
    
    long month = strtol(date.substr(5, 2).c_str(), &endptr, 10);
    if (*endptr != '\0') return false;
    
    long day = strtol(date.substr(8, 2).c_str(), &endptr, 10);
    if (*endptr != '\0') return false;

    if (year < 2009 || year > 2025)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 30)
        return false;
    return true;
}

bool BitcoinExchange::is_valid_value(float value)
{
    return value >= 0 && value <= 1000;
}

std::string BitcoinExchange::trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

void BitcoinExchange::handle_input(const char *input_file)
{
    std::ifstream file(input_file);
    if (!file.is_open())
    {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }
    std::string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;
        if (line.find("date") != std::string::npos)
            continue;
        size_t pipe = line.find("|");
        if (pipe == std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        std::string date = trim(line.substr(0, pipe));
        std::string value = trim(line.substr(pipe + 1));
        char* endptr;
        float fvalue = strtof(value.c_str(), &endptr);
        if (*endptr != '\0' && *endptr != '\n' && *endptr != '\r') {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        if (!is_valid_date(date))
        {
            std::cerr << "Error: bad input => " << date << std::endl;
            continue;
        }
        if (!is_valid_value(fvalue))
        {
            if (fvalue < 0)
            {
                std::cerr << "Error: not a positive number." << std::endl;
                continue;
            }
            if (fvalue > 1000)
            {
                std::cerr << "Error: too large a number." << std::endl;
                continue;
            }
        }
        float exchange_rate = get_exchange_rate(date);
        float result = exchange_rate * fvalue;
        if (exchange_rate >= 0)
            std::cout << date << " => " << fvalue << " = " << result << std::endl;
    }
}

float BitcoinExchange::get_exchange_rate(const std::string &date)
{
    std::map<std::string, float>::iterator it = database.find(date);
    if (it != database.end())
        return it->second;
    std::map<std::string, float>::iterator lb = database.lower_bound(date);
    if (lb == database.begin())
    {
        std::cerr << "Error: no previous date found" << std::endl;
        return -1;
    }
    lb--;
    return lb->second;
}