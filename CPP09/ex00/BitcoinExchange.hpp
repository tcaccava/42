#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <iostream>
#include <string>
#include <fstream>

class BitcoinExchange
{
public:
    BitcoinExchange();
    ~BitcoinExchange();
    BitcoinExchange(const BitcoinExchange &other);
    BitcoinExchange &operator=(const BitcoinExchange &other);

    bool create_database(const std::string &csv);
    bool is_valid_date(const std::string date);
    void handle_input(const char *input_file);
    bool is_valid_value(float value);
    float get_exchange_rate(const std::string &date);
    std::string trim(const std::string &s);

private:
    std::map<std::string, float> database;
};

#endif