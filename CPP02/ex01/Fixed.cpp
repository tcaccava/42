#include "Fixed.hpp"

Fixed::Fixed() : value(0)
{
	std::cout << "Default constructor called" << std::endl;
}

Fixed::Fixed(const int tofix)
{
	value = tofix << fractional_bits;
	std::cout << "Int constructor called" << std::endl;
}

Fixed::Fixed(const float tofix)
{
	value = roundf(tofix * 256);
	std::cout << "Float constructor called" << std::endl;
}

Fixed::Fixed(const Fixed &other)
{
	std::cout << "Copy constructor called" << std::endl;
	*this = other;
}

Fixed &Fixed::operator=(const Fixed &other)
{
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other)
		this->value = other.value;
	return (*this);
}
float Fixed::toFloat(void) const
{
	return (static_cast<float>(value) / 256);
}

int Fixed::toInt(void) const
{
	return (value >> fractional_bits);
}

Fixed::~Fixed()
{
	std::cout << "Destructor called" << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Fixed &fixed)
{
	os << fixed.toFloat();
	return (os);
}

