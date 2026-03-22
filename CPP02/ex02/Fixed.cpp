#include "Fixed.hpp"

const int Fixed::fractional_bits = 8;

Fixed::Fixed() : value(0){}

Fixed::Fixed(const int tofix) : value(0)
{
    value = tofix << fractional_bits;
}

Fixed::Fixed(const float tofix) : value(0)
{
    value = roundf(tofix * 256);
}

Fixed::Fixed(const Fixed &other)
{
    *this = other;
}

Fixed &Fixed::operator=(const Fixed &other)
{
    if (this != &other)
        this->value = other.value;
    return (*this);
}

bool Fixed::operator>(const Fixed &other) const
{
    return (this->value > other.value);
}

bool Fixed::operator>=(const Fixed &other) const
{
    return (this->value >= other.value);
}

bool Fixed::operator<(const Fixed &other) const
{
    return (this->value < other.value);
}

bool Fixed::operator<=(const Fixed &other) const
{
    return (this->value <= other.value);
}

bool Fixed::operator==(const Fixed &other) const
{
    return (this->value == other.value);
}

bool Fixed::operator!=(const Fixed &other) const
{
    return (this->value != other.value);
}

Fixed Fixed::operator+(const Fixed &other) const
{
    Fixed result;
    result.value = this->value + other.value;
    return (result);
}

Fixed Fixed::operator-(const Fixed &other) const
{
    Fixed result;
    result.value = (this->value - other.value);
    return (result);
}

Fixed Fixed::operator*(const Fixed &other) const
{
    Fixed result;
    result.value = (this->value * other.value) >> fractional_bits;
    return (result);
}

Fixed Fixed::operator/(const Fixed &other) const
{
    Fixed result;
    result.value = (this->value << fractional_bits) / other.value;
    return (result);
}

Fixed &Fixed::operator++()
{
    value++;
    return *this;
}
Fixed &Fixed::operator--()
{
    value--;
    return *this;
}

Fixed Fixed::operator++(int)
{
    Fixed temp = *this;
    value++;
    return temp;
}

Fixed Fixed::operator--(int)
{
    Fixed temp = *this;
    value--;
    return temp;
}

float Fixed::toFloat(void) const
{
    return (static_cast<float>(value) / 256);
}

Fixed::~Fixed(){}

std::ostream &operator<<(std::ostream &os, const Fixed &fixed)
{
    os << fixed.toFloat();
    return os;
}

Fixed &Fixed::min( Fixed &fixed1, Fixed &fixed2)
{
    return (fixed1 < fixed2) ? fixed1 : fixed2;
}
const Fixed &Fixed::min(const Fixed &fixed1, const Fixed &fixed2)
{
    return (fixed1 < fixed2) ? fixed1 : fixed2;
}
Fixed &Fixed::max( Fixed &fixed1, Fixed &fixed2)
{
    return (fixed1 > fixed2) ? fixed1 : fixed2;
}
const Fixed &Fixed::max(const Fixed &fixed1, const Fixed &fixed2)
{
    return (fixed1 > fixed2) ? fixed1 : fixed2;
}
