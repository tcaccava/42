#ifndef FIXED_HPP
#define FIXED_HPP

#include <iostream>
#include <string>
#include <cmath>

class Fixed
{
public:
	Fixed();
	Fixed(const int tofix);
	Fixed(const float tofix);
	Fixed(const Fixed &other);
	Fixed &operator=(const Fixed &other);
	~Fixed();

	bool operator>(const Fixed &other) const;
	bool operator>=(const Fixed &other) const;
	bool operator<=(const Fixed &other) const;
	bool operator==(const Fixed &other) const;
	bool operator<(const Fixed &other) const;
	bool operator!=(const Fixed &other) const;

	Fixed operator+(const Fixed &other) const;
	Fixed operator-(const Fixed &other) const;
	Fixed operator*(const Fixed &other) const;
	Fixed operator/(const Fixed &other) const;

	Fixed &operator++();
	Fixed operator++(int);
	Fixed &operator--();
	Fixed operator--(int);

	static Fixed &min(Fixed &fixed1, Fixed &fixed2);
	static const Fixed &min(const Fixed &fixed1, const Fixed &fixed2);
	static Fixed &max(Fixed &fixed1, Fixed &fixed2);
	static const Fixed &max(const Fixed &fixed1, const Fixed &fixed2);

	float toFloat(void) const;

private:
	int value;
	static const int fractional_bits;
};

std::ostream &operator<<(std::ostream &os, const Fixed &other);

#endif