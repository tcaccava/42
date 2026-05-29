#pragma once
#include <map>
#include <string>

class BitcoinExchange
{
private:
    std::map<std::string, double> db;
public:
    BitcoinExchange(BitcoinExchange &bce);
    BitcoinExchange &operator= (BitcoinExchange &bce);
    BitcoinExchange();
    ~BitcoinExchange();
    double convert(std::string date, double val);
};