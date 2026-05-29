#ifndef ConvertInt_HPP
# define ConvertInt_HPP

# include <iostream>
# include <string>
# include "AConverter.hpp"

class ConvertInt : public AConverter
{

	public:

		ConvertInt(const std::string & value);
		ConvertInt( ConvertInt const & src );
		~ConvertInt();

		ConvertInt &		operator=( ConvertInt const & rhs );

	private:
		ConvertInt();

		virtual void	printChar() const;
		virtual void	printInt() const;
		virtual void	printFloat() const;
		virtual void	printDouble() const;

		int _value;
};

std::ostream &			operator<<( std::ostream & o, ConvertInt const & i );

#endif