#ifndef FIXED_HPP
# define FIXED_HPP

# include <iostream>
# include <string>
#include <cmath>

class Fixed
{
  public:
	Fixed();
	~Fixed();
	Fixed(const int tofix);
	Fixed(const float tofix);
	Fixed(const Fixed &other);
	Fixed &operator=(const Fixed &other);
	float toFloat(void) const;
	int toInt(void) const;
	int getRawBits(void) const;
	void setRawBits(int const raw);

  private:
	static const int fractional_bits = 8;
	int value;
};

std::ostream& operator<<(std::ostream& os, const Fixed& fixed);


#endif
