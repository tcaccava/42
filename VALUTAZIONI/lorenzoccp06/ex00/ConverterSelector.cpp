# include "ConvertChar.hpp"
# include "ConvertDouble.hpp"
# include "ConverterSelector.hpp"
# include "ConvertInt.hpp"
# include "ConvertFloat.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ConverterSelector::ConverterSelector(const std::string & value) : _strValue(value) { }

ConverterSelector::ConverterSelector( const ConverterSelector & src ) { *this = src; }

ConverterSelector::~ConverterSelector() { }


ConverterSelector &				ConverterSelector::operator=( ConverterSelector const & rhs )
{
	if ( this != &rhs )
		this->_strValue = rhs._strValue;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ConverterSelector const & i )
{
	(void)i;
	o << "ConverterSelector" << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

/*
* Detect if is a char
* <char> OR '<char>'
*
* If it's a single char but also a number
* give priority to the number (and so return false).
* For char it's possible to use the format '<nbr>'.
*/
bool ConverterSelector::isChar() const {
	return ((_strValue.length() == 1 && !isdigit(_strValue[0]))
		|| (_strValue.length() == 3 && _strValue[0] == '\'' && _strValue[2] == '\''));
}

bool ConverterSelector::isPseudoFloat() const {
	return (_strValue == "-inff" || _strValue == "+inff" || _strValue == "nanf");
}

bool ConverterSelector::isPseudoDouble() const {
	return (_strValue == "-inf" || _strValue == "+inf" || _strValue == "nan");
}

ConverterSelector::LiteralType	ConverterSelector::detectType() const {
	bool		dot = false;
	bool		f = false;
	std::size_t	start;

	if (_strValue.empty()) return INVALID;
	if (isChar()) return CHAR;
	if (isPseudoFloat()) return FLOAT;
	if (isPseudoDouble()) return DOUBLE;

	// skip sign
	start = (_strValue[0] == '-' || _strValue[0] == '+') ? 1 : 0;
	// check if empty after sign
	if (_strValue.length() == start)
		return INVALID;
	// iterate single char
	for (std::size_t i = start; i < _strValue.length(); ++i) {
		if (_strValue[i] == '.' && !dot) // first dot: ok!
			dot = true;
		else if (_strValue[i] == 'f' && !f && i == _strValue.length() - 1) // last char is f: ok!
			f = true;
		else if (!isdigit(_strValue[i])) // otherwise must be a digit
			return INVALID;
	}
	if (!dot && f) return INVALID; // like "42f" is invalid
	if (f) return FLOAT; // has a dot and f
	if (dot) return DOUBLE; // has a dot but no f
	return INT; // no dot and no f
}

AConverter * ConverterSelector::getConverter() const {
	LiteralType type = detectType();
	switch (type) {
		case CHAR:
			return new ConvertChar(_strValue);
		case INT:
			return new ConvertInt(_strValue);
		case FLOAT:
			return new ConvertFloat(_strValue);
		case DOUBLE:
			return new ConvertDouble(_strValue);
		default:
			throw AConverter::InvalidConversion(_strValue);
	}
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */