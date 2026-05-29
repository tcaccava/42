#include "ScalarConverter.hpp"
#include "AConverter.hpp"
#include "ConverterSelector.hpp"

/*
** ---------------------------------------------------------------
*/

ScalarConverter::ScalarConverter() { }

ScalarConverter::ScalarConverter( const ScalarConverter & src ) { *this = src; }

ScalarConverter::~ScalarConverter() { }

ScalarConverter & ScalarConverter::operator=( ScalarConverter const & rhs ) {
	(void)rhs;
	return *this;
}

std::ostream & operator<<( std::ostream & o, ScalarConverter const & i )
{
	(void)i;
	o << "ScalarConverter" << std::endl;
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void ScalarConverter::convert( std::string const & str ) {
	AConverter * converter = NULL;

	try {
		ConverterSelector selector(str);
		converter = selector.getConverter();
		converter->printConvert();
	} catch (const AConverter::InvalidConversion & e) {
		std::cerr << "Error: " << e.what() << std::endl;
	} catch (const std::exception & e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown error" << std::endl;
	}
	delete converter;
}

/* ************************************************************************** */