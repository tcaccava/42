#include "ConvertFloat.hpp"
#include <iomanip>
#include <limits>
#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <complex>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

// ConvertFloat::ConvertFloat()
// {
// }

// https://cplusplus.com/reference/cstdlib/strtof/
ConvertFloat::ConvertFloat(const std::string & value) : AConverter(value) {
	// std::cout << "Float convertor" << std::endl;
	char * end;
	_value = strtof(value.c_str(), &end);
	// check if finish with f
	if (*end == 'f')
		end++;
	else
		throw AConverter::InvalidConversion(value);
	// check errors
	if (*end != '\0' || errno == ERANGE)
	{
		std::cerr << "End: " << *end << std::endl;
		std::cout << "errno: " << errno << std::endl;
		throw AConverter::InvalidConversion(value);
	}

}

ConvertFloat::ConvertFloat( const ConvertFloat & src ) : AConverter(src) {
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ConvertFloat::~ConvertFloat() { }


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ConvertFloat & ConvertFloat::operator=( ConvertFloat const & rhs ) {
	if ( this != &rhs ) {
		AConverter::operator=(rhs);
		this->_value = rhs._value;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

// check if nan or inf
bool ConvertFloat::isImpossible() const {
	return (_value == std::numeric_limits<float>::infinity()
		|| _value == -std::numeric_limits<float>::infinity()
		|| _value != _value); // check if is nan
}

void	ConvertFloat::printChar() const {
	// check if nan or inf
	if (isImpossible())
		std::cout << "impossible";
	// check if the value is in the range of char
	else if (_value > std::numeric_limits<char>::max()
		|| _value < std::numeric_limits<char>::min())
		std::cout << "impossible";
	else if (isprint(_value))
		std::cout << "'" << static_cast<char>(_value) << "'"; // explicit conversion
	else
		std::cout << "Non displayable";
}

void	ConvertFloat::printInt() const {
	// check if nan or inf
	if (isImpossible())
		std::cout << "impossible";
	// check if the value is in the range of int
	else if (_value > std::numeric_limits<int>::max()
		|| _value < std::numeric_limits<int>::min())
		std::cout << "impossible";
	else
		std::cout << static_cast<int>(_value);	// explicit conversion
}

void	ConvertFloat::printFloat() const {
	if (_value == std::numeric_limits<float>::infinity())
		std::cout << "+";
	
	// set float precision
	std::cout << std::setprecision(std::numeric_limits<float>::digits10)
		<< _value;

	const float tolerance = std::numeric_limits<float>::epsilon();
	if (std::fabs(_value - static_cast<int>(_value)) < tolerance)
		std::cout << ".0";
	std::cout << "f";
}

void	ConvertFloat::printDouble() const {
	/*
	*  nan, nanf -> nan
	*  -inf, -inff -> -inf
	*  +inf, +inff -> +inf
	*/
	if (_value == std::numeric_limits<float>::infinity())
		std::cout << "+";

	// set double precision
	double doubleVal = static_cast<double>(_value); // explicit conversion
	std::cout << std::setprecision(std::numeric_limits<double>::digits10)
		<< doubleVal;

	const double tolerance = std::numeric_limits<double>::epsilon();
	if (std::fabs(doubleVal - static_cast<int>(doubleVal)) < tolerance)
		std::cout << ".0";
}

/* ************************************************************************** */