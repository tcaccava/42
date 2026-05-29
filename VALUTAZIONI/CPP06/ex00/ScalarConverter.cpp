#include "ScalarConverter.hpp"

void ScalarConverter::convert(std::string input){
	char c;
	int i;
	float f;
	double d;

	if (input == "-inff" || input == "+inff" || input == "nanf") {
		f = static_cast<float>(std::atof(input.c_str()));
		d = static_cast<double>(f);
		std::cout << "char: impossible" << std::endl;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: " << f << "f" << std::endl;
        std::cout << "double: " << d << std::endl;
        return;
	}
	if (input == "-inf" || input == "+inf" || input == "nan") {
        d = std::atof(input.c_str());
        std::cout << "char: impossible" << std::endl ;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: " << static_cast<float>(d) << "f" << std::endl;
        std::cout << "double: " << d  << std::endl;
        return;
    }
	if (input.length() == 1 && input[0] >= 32 && input[0] <= 126 && !isdigit(input[0])){
		c = input[0];
		i = static_cast<int>(c);
		f = static_cast<float>(c);
		d = static_cast<double>(c);
		std::cout << "char: " << c << std::endl;
		std::cout << "int: " << i << std::endl;
		std::cout << std::fixed << std::setprecision(1) << "float " << f << "f" << std::endl;
    	std::cout << std::fixed << std::setprecision(1) << "double: " << d << std::endl;
		return;
	}

	char *endptr;
	errno = 0;
	long temp = std::strtol(input.c_str(), &endptr, 10);
	if (*endptr == '\0') {
        if (errno == ERANGE || temp < INT_MIN || temp > INT_MAX) {
            std::cout << "char: impossible" << std::endl;
            std::cout << "int: impossible (overflow)" << std::endl;
		    d = std::strtod(input.c_str(), &endptr);
            f = static_cast<float>(d);
            if (f > FLT_MAX || f < -FLT_MAX)
                std::cout << "float: impossible" << std::endl;
            else
                 std::cout << std::fixed << std::setprecision(1) << "float: " << f << "f" << std::endl;
            if (d > DBL_MAX || d < -DBL_MAX)
                std::cout << "double: impossible" << std::endl;
            else
                std::cout << std::fixed << std::setprecision(1) << "double: " << d << std::endl;
            return;
        }
        i = static_cast<int>(temp);
        f = static_cast<float>(i);
        d = static_cast<double>(i);
        c = static_cast<char>(i);

        if (i >= 32 && i <= 126)
            std::cout << "char: '" << c << "'" << std::endl;
        else
            std::cout << "char: Non displayable" << std::endl;

        std::cout << "int: " << i << std::endl;
        std::cout << std::fixed << std::setprecision(1) << "float: " << f << "f" << std::endl;
        std::cout << std::fixed << std::setprecision(1) << "double: " << d << std::endl;
        return;
    }

    errno = 0;
    d = std::atof(input.c_str());
    f = static_cast<float>(d);

    if (errno == ERANGE || d > DBL_MAX || d < -DBL_MAX) {
        std::cout << "char: impossible" << std::endl;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: impossible (overflow)" << std::endl;
        std::cout << "double: impossible (overflow)" << std::endl;
        return;
    }

    if (f > FLT_MAX || f < -FLT_MAX) {
        std::cout << "char: impossible" << std::endl;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: impossible (overflow)" << std::endl;
        std::cout << std::fixed << std::setprecision(1) << "double: " << d << std::endl;
        return;
    }

    i = static_cast<int>(d);
    c = static_cast<char>(i);

    if(!isdigit(input[0]) && input[0] != '-')
    {
        std::cout << "char: impossible" << std::endl;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: impossible" << std::endl;
        std::cout << "double: impossible" << std::endl;
        return;
    }

    if (i >= 32 && i <= 126)
        std::cout << "char: '" << c << "'" << std::endl;
    else
        std::cout << "char: Non displayable" << std::endl;

    if (i <= INT_MIN || i > INT_MAX)
        std::cout << "int: impossible (overflow)" << std::endl;
    else
        std::cout << "int: " << i << std::endl;
    std::cout << std::fixed << std::setprecision(1) << "float: " << f << "f" << std::endl;
    std::cout << std::fixed << std::setprecision(1) << "double: " << d << std::endl;
}

ScalarConverter::ScalarConverter(){
}

ScalarConverter::ScalarConverter(const ScalarConverter &other){
	(void) other;
}
ScalarConverter& ScalarConverter::operator=(const ScalarConverter &other){
	(void) other;
	return *this;
}

ScalarConverter::~ScalarConverter(){
}