#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>

BitcoinExchange::BitcoinExchange(BitcoinExchange &bce)
{
    *this = bce;
}

BitcoinExchange & BitcoinExchange::operator= (BitcoinExchange &bce)
{
    db = bce.db;
    return *this;
}


BitcoinExchange::BitcoinExchange()
{
    std::ifstream in("data.csv");
    if(!in.good())
    {
        throw std::runtime_error("No such file: data.csv");
    }
    
    std::string s;
    std::getline(in, s);
    while(!in.eof())
    {
        std::string date;
        double val;
        std::getline(in, date, ',');
        if (in.eof())
            break;
        in >> val;
        if (in.fail())
            throw std::runtime_error("data.csv: format error");
        db[date] = val;
        // std::cout << date << " : " << val << std::endl;
        std::getline(in, s);
    }
}

BitcoinExchange::~BitcoinExchange() {}

double BitcoinExchange::convert(std::string date, double val)
{
    double rate = 0;
    std::map<std::string, double>::iterator it = db.upper_bound(date);
    if (it != db.begin())
    {
        rate = (--it)->second;
    }
    return val*rate;

}
