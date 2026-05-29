/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:43:55 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 09:11:59 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"
#include <iostream>
#include <cmath>

Fixed::Fixed() {
	rawBits = 0;
}

Fixed::Fixed( int nbr ) {
	rawBits = nbr << fractionalBits;
}

Fixed::Fixed( float nbr ) {
	rawBits = static_cast<int>(roundf(nbr * (1 << fractionalBits)));
}

Fixed::Fixed(const Fixed& Fixed ) {
	rawBits = Fixed.rawBits;
}

Fixed& Fixed::operator=(const Fixed& Fixed) {
	if (this != &Fixed) {
		rawBits = Fixed.rawBits;
	}
	return (*this);
}

std::ostream& operator<<(std::ostream& out, const Fixed& fixedPoint) {
	out << fixedPoint.toFloat();
	return out;
}

Fixed	Fixed::operator+( const Fixed& fixedPoint ) const {
	Fixed	newFixedPoint;

	newFixedPoint.setRawBits(this->getRawBits() + fixedPoint.getRawBits());
	return (newFixedPoint);
}

Fixed	Fixed::operator-( const Fixed& fixedPoint ) const {
	Fixed	newFixedPoint;

	newFixedPoint.setRawBits(this->getRawBits() - fixedPoint.getRawBits());
	return (newFixedPoint);
}

Fixed	Fixed::operator*( const Fixed& fixedPoint ) const {
	Fixed result;
	int64_t	product;
	int		scaledValue;

	product = static_cast<int64_t>(this->rawBits) * \
				static_cast<int64_t>(fixedPoint.rawBits);

	scaledValue = static_cast<int>(product >> Fixed::fractionalBits);

	result.setRawBits(scaledValue);
	return (result);
}

Fixed	Fixed::operator/( const Fixed& fixedPoint ) const {
	Fixed	result;
	int64_t	numerator;
	int64_t	denominator;
	int		scaled;

	numerator = static_cast<int64_t>(this->rawBits) << Fixed::fractionalBits;
	denominator = static_cast<int64_t>(fixedPoint.rawBits);
	scaled = static_cast<int>(numerator / denominator);

	result.setRawBits(scaled);
	return (result);
}

Fixed&		Fixed::operator++( ) {
	this->rawBits += 1;
	return (*this);
}

Fixed		Fixed::operator++( int ) {
	Fixed	tmp(*this);

	rawBits += 1;
	return (tmp);
}

Fixed&		Fixed::operator--( ) {
	this->rawBits -= 1;
	return (*this);
}

Fixed		Fixed::operator--( int ) {
	Fixed	tmp(*this);

	rawBits -= 1;
	return (tmp);
}

int	Fixed::getRawBits( void ) const {
	return (rawBits);
}

void	Fixed::setRawBits( int const raw ) {
	rawBits = raw;
}

float	Fixed::toFloat( void ) const {
	return (static_cast<float>(rawBits) / (1 << fractionalBits));
}

int		Fixed::toInt( void ) const {
	return (rawBits >> fractionalBits);
}

const Fixed&	Fixed::min(const Fixed& fixed1, const Fixed& fixed2) {
	if (fixed1.getRawBits() > fixed2.getRawBits()) {
		return (fixed2);
	}
	return (fixed1);
}

Fixed&	Fixed::min( Fixed& fixed1, Fixed& fixed2) {
	if (fixed1.getRawBits() > fixed2.getRawBits()) {
		return (fixed2);
	}
	return (fixed1);
}

const Fixed&	Fixed::max(const Fixed& fixed1, const Fixed& fixed2) {
	if (fixed1.getRawBits() < fixed2.getRawBits()) {
		return (fixed2);
	}
	return (fixed1);
}

Fixed&	Fixed::max(Fixed& fixed1, Fixed& fixed2) {
	if (fixed1.getRawBits() < fixed2.getRawBits()) {
		return (fixed2);
	}
	return (fixed1);
}

bool	Fixed::operator>( const Fixed& fixedPoint ) const {
	return (this->getRawBits() > fixedPoint.getRawBits());
}

bool	Fixed::operator<( const Fixed& fixedPoint ) const {
	return (this->getRawBits() < fixedPoint.getRawBits());
}

bool	Fixed::operator>=( const Fixed& fixedPoint ) const {
	return (this->getRawBits() >= fixedPoint.getRawBits());
}

bool	Fixed::operator<=( const Fixed& fixedPoint ) const {
	return (this->getRawBits() <= fixedPoint.getRawBits());
}

bool	Fixed::operator==( const Fixed& fixedPoint ) const {
	return (this->getRawBits() == fixedPoint.getRawBits());
}

bool	Fixed::operator!=( const Fixed& fixedPoint ) const {
	return (this->getRawBits() != fixedPoint.getRawBits());
}

Fixed::~Fixed() {
}
