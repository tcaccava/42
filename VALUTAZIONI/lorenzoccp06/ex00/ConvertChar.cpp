#include "ConvertChar.hpp"
#include <iomanip>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

// ConvertChar::ConvertChar()
// {
// }

ConvertChar::ConvertChar(const std::string & value) : AConverter(value) {
	// std::cout << "Char convertor" << std::endl;
	if (value.length() == 1 && !isdigit(value[0]))
		_value = static_cast<char>(value[0]);
	else if (value.length() == 3 && value[0] == '\'' && value[2] == '\'')
		_value = static_cast<char>(value[1]);
	else
		throw AConverter::InvalidConversion(value);
}


ConvertChar::ConvertChar( const ConvertChar & src ) : AConverter(src) {
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ConvertChar::~ConvertChar() { }


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ConvertChar & ConvertChar::operator=( ConvertChar const & rhs ) {
	if ( this != &rhs ) {
		AConverter::operator=(rhs);
		this->_value = rhs._value;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	ConvertChar::printChar() const {
	if (isprint(_value))
		std::cout << "'" << _value << "'";
	else
		std::cout << "Non displayable";
}

void	ConvertChar::printInt() const {
	std::cout << static_cast<int>(_value);	// explicit conversion
}

void	ConvertChar::printFloat() const {
	std::cout << std::fixed << std::setprecision(1)
		<< static_cast<float>(_value)		// explicit conversion
		<< "f";
}

void	ConvertChar::printDouble() const {
	std::cout << std::fixed << std::setprecision(1)
		<< static_cast<double>(_value);		// explicit conversion
}

/* ************************************************************************** */