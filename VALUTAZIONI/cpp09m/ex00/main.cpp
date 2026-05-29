#include "BitcoinExchange.hpp"
#include <iostream>
#include <exception>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>

using std::cout;
using std::endl;

std::string strip(const std::string &s)
{
    int end = s.length() - 1;
    int beg = 0;
    while(beg <=  end && strchr(" \n\r", s[beg]))
        beg++;
    
    while(end > beg && strchr(" \n\r", s[end] ))
        end--;
    if (beg > end)
        return "";
    return s.substr(beg, end - beg + 1);
}

bool checkDate(std::string date)
{
    std::stringstream ss(date);
    int y, m, d;
    char delim1, delim2;
    ss >> y >> delim1 >> m >> delim2 >> d;
    if(ss.fail() || delim1 != '-' || delim2 != '-')
        return false;
    return 2000 <= y && 1 <= m && m <= 12 && 1 <= d && d <= 31;
}

void print_conversion(std::string date, double val, BitcoinExchange &b)
{
    if (!checkDate(date))
    {
        std::cout << "Bad date: " << date << std::endl;
    }
    else if(val < 0)
    {
        std::cout << "value can't be negative" << std::endl;
    }
    else if(val > 1000)
    {
        std::cout << "value can't be above 1000" << std::endl;
    }
    else
    {
        std::cout << date <<" => " << val << " = " <<\
            b.convert(date, val) << std::endl;
    }
}


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage:"<<argv[0]<<" FILE.txt\n";
        return 1;
    }
    BitcoinExchange b;
    
    std::ifstream in(argv[1]);
    if(!in.good())
    {
        throw std::runtime_error(std::string("No such file: ")+argv[1]);
    }
    
    std::string s;
    std::getline(in, s); // header
    
    while(!in.eof())
    {
        std::getline(in, s);
        // cout << "s:" << s << std::endl;
        char *str = strdup(s.c_str());
        char *rest;
        char *rest2;
        std::string date = strip(strtok_r(str, "|", &rest));
        double val = std::strtod(rest, &rest2);
        if (rest == rest2)
            cout << "Format error, missing value field" << std::endl;
        else if (strip(rest2).size() > 0)
        {
            cout << "Format error, unexpected \"" << rest2 << "\" after the number" << endl;
        }
        else 
        {
            print_conversion(date, val, b);
        }
        free(str);
    }

    
}