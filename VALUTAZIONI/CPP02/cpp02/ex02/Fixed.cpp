/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:31:17 by mla-mona          #+#    #+#             */
/*   Updated: 2024/08/30 16:01:51 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

Fixed::Fixed() : fixedPoint(0)
{
    std::cout << "Default contructor called" << std::endl;
}

Fixed::Fixed(const Fixed &other)
{
    std::cout << "Copy constructor called" << std::endl;
    *this = other; //chiama l'operatore di assegnazione
}

Fixed &Fixed::operator = (const Fixed &other)
{
    std::cout << "Copy assignment operator called" << std::endl;
    if(this != &other)
    {
        this->fixedPoint = other.getRawBits();
    }
    return *this;
}

Fixed::~Fixed() 
{
    std::cout << "Destructor called" << std::endl;
}

// integer constructor
Fixed::Fixed(const int intValue)
{
    std::cout << "Int constructor called" << std::endl;
    fixedPoint = intValue << fractionalBits;
}

// Floating-point constructor
Fixed::Fixed(float const floatValue)
{
    std::cout << "Float constructor called" << std::endl;
    fixedPoint = roundf(floatValue * (1 << fractionalBits));
}

int Fixed::getRawBits(void) const
{
    std::cout << "getRawBits member function called" << std::endl;
    return this->fixedPoint;
}

void Fixed::setRawBits(int const raw)
{
    std::cout << "setRawBits member function called" << std::endl;
    this->fixedPoint = raw;
}

// Member function to convert to float
float Fixed::toFloat(void) const
{
    return static_cast<float>(fixedPoint) / (1 << fractionalBits);
}

// Member function to convert to int
int Fixed::toInt(void) const
{
    return fixedPoint >> fractionalBits;
}

// Overload of the insertion (<<) operator
std::ostream &operator<<(std::ostream &out, const Fixed &fixed)
{    out << fixed.toFloat();
    return out;
}

// Comparison operators
bool Fixed::operator>(const Fixed &other) const
{
        return this->fixedPoint > other.fixedPoint;
}

bool Fixed::operator<(const Fixed &other) const
{
    return this->fixedPoint < other.fixedPoint;
}

bool Fixed::operator>=(const Fixed &other) const
{
    return this->fixedPoint >= other.fixedPoint;
}

bool Fixed::operator<=(const Fixed &other) const
{
    return this->fixedPoint <= other.fixedPoint;
}

bool Fixed::operator==(const Fixed &other) const
{
    return this->fixedPoint == other.fixedPoint;
}

bool Fixed::operator!=(const Fixed &other) const
{
    return this->fixedPoint != other.fixedPoint;
}

// Arithmetic operators
Fixed Fixed::operator+(const Fixed &other) const
{
    return Fixed(this->toFloat() + other.toFloat());
}

Fixed Fixed::operator-(const Fixed &other) const
{
    return Fixed(this->toFloat() - other.toFloat());
}

Fixed Fixed::operator*(const Fixed &other) const
{
    return Fixed(this->toFloat() * other.toFloat());
}

Fixed Fixed::operator/(const Fixed &other) const
{
    return Fixed(this->toFloat() / other.toFloat());
}

// Increment/Decrement operators
Fixed &Fixed::operator++()
{
    ++this->fixedPoint;
    return *this;
}

Fixed Fixed::operator++(int)
{
    Fixed temp = *this;
    ++(*this);
    return temp;
}

Fixed &Fixed::operator--()
{
    --this->fixedPoint;
    return *this;
}

Fixed Fixed::operator--(int)
{
    Fixed temp = *this;
    --(*this);
    return temp;
}

// Static member functions
Fixed &Fixed::min(Fixed &a, Fixed &b)
{
    return (a < b) ? a : b;
}

const Fixed &Fixed::min(const Fixed &a, const Fixed &b)
{
    return (a < b) ? a : b;
}

Fixed &Fixed::max(Fixed &a, Fixed &b)
{
    return (a > b) ? a : b;
}

const Fixed &Fixed::max(const Fixed &a, const Fixed &b)
{
    return (a > b) ? a : b;
}
