#ifndef ConvertFloat_HPP
# define ConvertFloat_HPP

# include <iostream>
# include <string>
# include "AConverter.hpp"

class ConvertFloat : public AConverter
{

	public:

		ConvertFloat(const std::string & value);
		ConvertFloat( ConvertFloat const & src );
		~ConvertFloat();

		ConvertFloat &		operator=( ConvertFloat const & rhs );

	private:
		ConvertFloat();

		bool isImpossible() const;

		virtual void	printChar() const;
		virtual void	printInt() const;
		virtual void	printFloat() const;
		virtual void	printDouble() const;

		float _value;
};

std::ostream &			operator<<( std::ostream & o, ConvertFloat const & i );

#endif