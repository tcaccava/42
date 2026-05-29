#include "ConvertInt.hpp"
#include <iomanip>
#include <limits>
#include <cerrno>
#include <iostream>
#include <cstdlib>

/*
** ------------------------------- CONSTRUCTOR --------------------------------dd
*/

// ConvertInt::ConvertInt()
// {
// }

// https://cplusplus.com/reference/cstdlib/strtol/
ConvertInt::ConvertInt(const std::string & value) : AConverter(value) {
	// std::cout << "Int convertor" << std::endl;
	// first convert as a long
	char * end;
	long temp_value = strtol(value.c_str(), &end, 10);
	// check errors
	if (*end != '\0' || errno == ERANGE)
		throw AConverter::InvalidConversion(value);
	// check if the value is in the range of int
	if (temp_value > std::numeric_limits<int>::max()
		|| temp_value < std::numeric_limits<int>::min())
		// out of range => long => type not supported
		throw AConverter::InvalidConversion(value);
	// convert to int
	_value = static_cast<int>(temp_value);
}


ConvertInt::ConvertInt( const ConvertInt & src ) : AConverter(src) {
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ConvertInt::~ConvertInt() { }


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ConvertInt & ConvertInt::operator=( ConvertInt const & rhs ) {
	if ( this != &rhs ) {
		AConverter::operator=(rhs);
		this->_value = rhs._value;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	ConvertInt::printChar() const {
	// check if the value is in the range of char
	if (_value > std::numeric_limits<char>::max()
		|| _value < std::numeric_limits<char>::min())
		std::cout << "impossible";
	else if (isprint(_value))
		std::cout << "'" << static_cast<char>(_value) << "'"; // explicit conversion
	else
		std::cout << "Non displayable";
}

void	ConvertInt::printInt() const {
	std::cout << _value;
}

void	ConvertInt::printFloat() const {
	std::cout << std::fixed << std::setprecision(1)
		<< static_cast<float>(_value)		// explicit conversion
		<< "f";
}

void	ConvertInt::printDouble() const {
	std::cout << std::fixed << std::setprecision(1)
		<< static_cast<double>(_value);		// explicit conversion
}

/* ************************************************************************** */