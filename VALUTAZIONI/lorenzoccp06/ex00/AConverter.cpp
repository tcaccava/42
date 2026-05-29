#include "AConverter.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

// AConverter::AConverter()
// {
// }

AConverter::AConverter(const std::string & value) : _strValue(value) { }

AConverter::AConverter( const AConverter & src ) {
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AConverter::~AConverter() { }


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AConverter &				AConverter::operator=( AConverter const & rhs )
{
	if ( this != &rhs )
		this->_strValue = rhs._strValue;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, AConverter const & i )
{
	o << "AConverter" << i.getValue() << std::endl;
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void AConverter::printConvert() const {
	std::cout << "char: ";
	this->printChar();
	std::cout << std::endl <<  "int: ";
	this->printInt();
	std::cout << std::endl <<  "float: ";
	this->printFloat();
	std::cout << std::endl <<  "double: ";
	this->printDouble();
	std::cout << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string AConverter::getValue() const {
	return this->_strValue;
}

/* ************************************************************************** */