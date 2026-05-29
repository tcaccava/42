#ifndef ConvertDouble_HPP
# define ConvertDouble_HPP

# include <iostream>
# include <string>
# include "AConverter.hpp"

class ConvertDouble : public AConverter
{
	public:

		ConvertDouble(const std::string & value);
		ConvertDouble( ConvertDouble const & src );
		~ConvertDouble();

		ConvertDouble &		operator=( ConvertDouble const & rhs );

	private:
		ConvertDouble();

		virtual void	printChar() const;
		virtual void	printInt() const;
		virtual void	printFloat() const;
		virtual void	printDouble() const;

		bool isImpossible() const;

		double _value;
};

std::ostream &			operator<<( std::ostream & o, ConvertDouble const & i );

# include "AConverter.hpp"

#endif