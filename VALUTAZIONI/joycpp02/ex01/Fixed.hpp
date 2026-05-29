/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:50:25 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/21 11:05:09 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP

#include <iostream>
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

};

std::ostream &operator << (std::ostream &out, const Fixed &fixed);

#endif