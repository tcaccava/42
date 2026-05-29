/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:56:15 by mla-mona          #+#    #+#             */
/*   Updated: 2024/08/30 15:20:59 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP

#include <string>
#include <iostream>
#include <cmath>

class Fixed
{
    private:
        int fixedPoint;
        static const int fractionalBits = 8;
    public:
        //default constructor
        Fixed();
        //copy constructor
        Fixed(const Fixed &other);
        //copy assignment operator
        Fixed &operator = (const Fixed &other);
        //destructor
        ~Fixed();

        Fixed(const int intValue);
        Fixed(const float floatValue);

        
        int getRawBits(void) const;
        void setRawBits(int const raw);

        float toFloat(void) const;
        int toInt(void) const;
};

// Overload of the insertion (<<) operator
std::ostream &operator<<(std::ostream &out, const Fixed &fixed);

#endif