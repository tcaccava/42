/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:54:03 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/21 20:08:16 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP

#include <iostream>
#include <ostream>
#include <cmath>

class Fixed {
private:
    int fixed;
    const static int bits = 8;
public:
    Fixed();
    Fixed(const Fixed &instance);
    Fixed& operator=(const Fixed &instance);
    ~Fixed();

    Fixed(const int num);
    Fixed(const float num);
    
    int getRawBits( void ) const;
    void setRawBits( int const raw );
    
    float toFloat( void ) const;
    int toInt( void ) const;
    
    bool operator > (const Fixed &arg2) const;
    bool operator < (const Fixed &arg2) const;
    bool operator >= (const Fixed &arg2) const;
    bool operator <= (const Fixed &arg2) const;
    bool operator == (const Fixed &arg2) const;
    bool operator != (const Fixed &arg2) const;

    //to return float
    /*float operator + (const Fixed &arg2) const;
    float operator - (const Fixed &arg2) const;
    float operator * (const Fixed &arg2) const;
    float operator / (const Fixed &arg2) const;*/

    Fixed operator + (const Fixed &arg2) const;
    Fixed operator - (const Fixed &arg2) const;
    Fixed operator * (const Fixed &arg2) const;
    Fixed operator / (const Fixed &arg2) const;
    

    Fixed &operator ++();
    Fixed &operator --();
    Fixed operator ++(int);
    Fixed operator --(int);

    static Fixed &min(Fixed &arg1, Fixed &arg2);
    const static Fixed &min(const Fixed &arg1, const Fixed &arg2);

    static Fixed &max(Fixed &arg1, Fixed &arg2);
    const static Fixed &max(const Fixed &arg1, const Fixed &arg2);
};

std::ostream &operator << (std::ostream &out, const Fixed &fixed);

#endif