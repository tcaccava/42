#ifndef ConvertChar_HPP
# define ConvertChar_HPP

# include <iostream>
# include <string>
# include "AConverter.hpp"

class ConvertChar : public AConverter
{

	public:

		ConvertChar(const std::string & value);
		ConvertChar( ConvertChar const & src );
		~ConvertChar();

		ConvertChar &		operator=( ConvertChar const & rhs );

	private:
		ConvertChar();

		virtual void	printChar() const;
		virtual void	printInt() const;
		virtual void	printFloat() const;
		virtual void	printDouble() const;

		char _value;
};

std::ostream &			operator<<( std::ostream & o, ConvertChar const & i );

#endif