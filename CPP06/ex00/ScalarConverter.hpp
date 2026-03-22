#ifndef SCALARCONVERTER_HPP
#define SCALARCONVERTER_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <climits>
#include <cerrno>
#include <iomanip>
#include <limits>
#include <cmath>

class ScalarConverter {
    private:
        ScalarConverter();
        ~ScalarConverter();
        ScalarConverter(const ScalarConverter& other);
        ScalarConverter &operator=(const ScalarConverter& other);

        static bool isCharLiteral(const std::string& literal);
        static void handleCharLiteral(char c);

        static bool isIntLiteral(const std::string &literal);
        static void handleIntLiteral(long value);

        static bool isFloatLiteral(const std::string& literal);
        static void handleFloatLiteral(float value,const std::string& literal);

        static bool isDoubleLiteral(const std::string& literal);
        static void handleDoubleLiteral(double value,const std::string &literal);
    
    public :
        static void convert(const std::string &literal);
};

#endif