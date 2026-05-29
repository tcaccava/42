#include "ConvertDouble.hpp"
#include <limits>
#include <complex>
#include <cerrno>
#include <iomanip>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

// ConvertDouble::ConvertDouble()
// {
// }

// https://cplusplus.com/reference/cstdlib/strtod/
ConvertDouble::ConvertDouble(const std::string & value) : AConverter(value) {
	char *end;

	// std::cout << "Double convertor" << std::endl;
	_value = strtod(value.c_str(), &end);
	if (*end != '\0' || errno == ERANGE)
		throw AConverter::InvalidConversion(value);
}


ConvertDouble::ConvertDouble( const ConvertDouble & src ) : AConverter(src) {
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ConvertDouble::~ConvertDouble() { }


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ConvertDouble & ConvertDouble::operator=( ConvertDouble const & rhs ) {
	if ( this != &rhs ) {
		AConverter::operator=(rhs);
		this->_value = rhs._value;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool ConvertDouble::isImpossible() const {
	return (_value == std::numeric_limits<double>::infinity()
		|| _value == -std::numeric_limits<double>::infinity()
		|| _value != _value); // check if is nan
}

void	ConvertDouble::printChar() const {
	// nan or inf is not a char
	if (isImpossible())
		std::cout << "impossible";
	// check if the value is in the range of char
	else if (_value > std::numeric_limits<char>::max()
		|| _value < std::numeric_limits<char>::min())
		std::cout << "impossible";
	else if (isprint(static_cast<char>(_value)))
		std::cout << "'" << static_cast<char>(_value) << "'"; // explicit conversion
	else
		std::cout << "Non displayable";
}

void	ConvertDouble::printInt() const {
	// nan or inf is not an int
	if (isImpossible())
		std::cout << "impossible";
	// check if the value is in the range of int
	else if (_value > std::numeric_limits<int>::max()
		|| _value < std::numeric_limits<int>::min())
		std::cout << "impossible";
	else
		std::cout << static_cast<int>(_value);
}

void	ConvertDouble::printFloat() const {
	if (_value == std::numeric_limits<double>::infinity())
		std::cout << "+";

	float floatValue = static_cast<float>(_value); // explicit conversion

	std::cout << std::setprecision(std::numeric_limits<float>::digits10)
		<< floatValue;

	const float tolerance = std::numeric_limits<float>::epsilon();
	if (std::fabs(floatValue - static_cast<int>(floatValue)) < tolerance)
		std::cout << ".0";
	std::cout << "f";
}

void	ConvertDouble::printDouble() const {
	/*
	*  nan, nanf -> nan
	*  -inf, -inff -> -inf
	*  +inf, +inff -> +inf
	*/
	if (_value == std::numeric_limits<double>::infinity())
		std::cout << "+";

	std::cout << std::setprecision(std::numeric_limits<double>::digits10)
		<< _value;

	const double tolerance = std::numeric_limits<double>::epsilon();
	if (std::fabs(_value - static_cast<int>(_value)) < tolerance)
		std::cout << ".0";
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */