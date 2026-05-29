#ifndef ConverterSelector_HPP
# define ConverterSelector_HPP

# include <iostream>
# include <string>

class AConverter;

class ConverterSelector
{

	public:
		ConverterSelector(const std::string & value);
		ConverterSelector( ConverterSelector const & src );
		~ConverterSelector();

		ConverterSelector &		operator=( ConverterSelector const & rhs );

		AConverter *	getConverter() const;

	private:
		enum LiteralType {
			CHAR,
			INT,
			FLOAT,
			DOUBLE,
			INVALID
		};

		ConverterSelector();

		std::string _strValue;
		
		LiteralType detectType() const;
		bool isChar() const;
		bool isPseudoFloat() const;
		bool isPseudoDouble() const;
};

std::ostream &			operator<<( std::ostream & o, ConverterSelector const & i );

#endif /* ************************************************ ConverterSelector_H */