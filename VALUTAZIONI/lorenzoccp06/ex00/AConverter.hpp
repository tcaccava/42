#ifndef ACONVERTER_HPP
# define ACONVERTER_HPP

# include <iostream>
# include <string>

class AConverter
{

	public:

		AConverter(const std::string & value);
		AConverter( AConverter const & src );
		virtual ~AConverter();

		AConverter &		operator=( AConverter const & rhs );

		void	printConvert() const;

		std::string getValue() const;

		// Exceptions classes
		// https://stackoverflow.com/questions/23742448/c-how-to-pass-parameters-to-custom-exceptions/23742555
		class InvalidConversion : public std::exception {
			private:
				std::string _msg;

			public:
				InvalidConversion(const std::string & value)
					: _msg("Invalid conversion: " + value) { }

				virtual const char* what() const throw() {
					return _msg.c_str();
				}

				virtual ~InvalidConversion() throw() { }
		};

	protected:

		virtual void	printChar() const = 0;
		virtual void	printInt() const = 0;
		virtual void	printFloat() const = 0;
		virtual void	printDouble() const = 0;
	
	private:
		AConverter();

		std::string _strValue;

};

std::ostream &			operator<<( std::ostream & o, AConverter const & i );

#endif /* ****************************************************** ACONVERTER_H */