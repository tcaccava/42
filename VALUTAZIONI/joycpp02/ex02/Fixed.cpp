/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:53:09 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/22 18:17:40 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

Fixed::Fixed()
{
    //std::cout << "Default constructor called" << std::endl;
    this->fixed = 0;
}

Fixed::Fixed(const Fixed &obj)
{
    //std::cout << "Copy constructor called" << std::endl;
    *this = obj;
}

Fixed& Fixed::operator=(const Fixed &obj)
{
    //std::cout << "Copy assignment operator called" << std::endl;
    if (this != &obj)
        this->fixed = obj.fixed;
    return *this;
}

Fixed::~Fixed()
{
    //std::cout << "Destructor called" << std::endl;
}
int Fixed:: getRawBits( void ) const
{
    //std::cout << "getRawBits member function called" << std::endl;
    return this->fixed;
}

void Fixed::setRawBits( int const raw )
{
    std::cout << "setRawBits member function called" << std::endl;
    this->fixed = raw;
}

Fixed::Fixed(const int num)
{
    //std::cout << "Int constructor called" << std::endl;
    this->fixed = num * (1 << this->bits);
}
Fixed::Fixed(const float num)
{
    //std::cout << "Float constructor called" << std::endl;
    this->fixed = roundf(num * (1 << this->bits));
}

int Fixed::toInt( void ) const
{
    int ret;

    ret = this->fixed / (1 << this->bits);
    return ret;
}

float Fixed::toFloat( void ) const
{
    float ret;

    ret = (float)this->fixed / (1 << this->bits);
    return ret;
}

std::ostream &operator << (std::ostream &out, const Fixed &fixed)
{
    out << fixed.toFloat();
    return out;
}

bool Fixed::operator < (const Fixed &arg2) const
{
    if(this->getRawBits() < arg2.getRawBits())
        return true;
    return false;
}

bool Fixed::operator > (const Fixed &arg2) const
{
    if(this->getRawBits() > arg2.getRawBits())
        return true;
    return false;
}

bool Fixed::operator >= (const Fixed &arg2) const
{
    if(this->getRawBits() >= arg2.getRawBits())
        return true;
    return false;
}

bool Fixed::operator <= (const Fixed &arg2) const
{
    if(this->getRawBits() <= arg2.getRawBits())
        return true;
    return false;
}

bool Fixed::operator == (const Fixed &arg2) const
{
    if(this->getRawBits() == arg2.getRawBits())
        return true;
    return false;
}

bool Fixed::operator != (const Fixed &arg2) const
{
    if(this->getRawBits() != arg2.getRawBits())
        return true;
    return false;
}
// return float
/*float Fixed::operator + (const Fixed &arg2) const
{
    return this->toFloat() + arg2.toFloat();
}

float Fixed::operator - (const Fixed &arg2) const
{
    return this->toFloat() - arg2.toFloat();
}

float Fixed::operator * (const Fixed &arg2) const
{
    return this->toFloat() * arg2.toFloat();
}

float Fixed::operator / (const Fixed &arg2) const
{
    return this->toFloat() / arg2.toFloat();
}*/

// return fixed
Fixed Fixed::operator + (const Fixed &arg2) const 
{
    return Fixed(this->toFloat() + arg2.toFloat());
}

Fixed Fixed::operator - (const Fixed &arg2) const 
{
    return Fixed(this->toFloat() - arg2.toFloat());
}

Fixed Fixed::operator * (const Fixed &arg2) const 
{
    return Fixed(this->toFloat() * arg2.toFloat());
}

Fixed Fixed::operator / (const Fixed &arg2) const 
{
    return Fixed(this->toFloat() / arg2.toFloat());
}

Fixed &Fixed::operator ++()
{
    this->fixed++;
    return *this;
}

Fixed &Fixed::operator--()
{
    this->fixed--;
    return *this;
}

Fixed Fixed::operator ++(int)
{
    Fixed tmp(*this);
    this->fixed++;
    return tmp;
}

Fixed Fixed::operator --(int)
{
    Fixed tmp(*this);
    this->fixed--;
    return tmp;
}

Fixed &Fixed::min(Fixed &arg1, Fixed &arg2)
{
    if(arg1 < arg2)
        return arg1;
    return arg2;
}
const Fixed &Fixed::min(const Fixed &arg1, const Fixed &arg2)
{
    if(arg1 < arg2)
        return arg1;
    return arg2;
}

Fixed &Fixed::max(Fixed &arg1, Fixed &arg2)
{
    if(arg1 > arg2)
        return arg1;
    return arg2;
}

const Fixed &Fixed::max(const Fixed &arg1, const Fixed &arg2)
{
    if(arg1 > arg2)
        return arg1;
    return arg2;
}