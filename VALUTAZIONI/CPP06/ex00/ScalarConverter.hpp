#pragma once
#include <string>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <climits>
#include <cerrno>
#include <cfloat>

class ScalarConverter{
private:
	ScalarConverter();
	ScalarConverter(const ScalarConverter &other);
	ScalarConverter& operator=(const ScalarConverter &otoher);
	~ScalarConverter();
public:
	static void convert(std::string input);
};