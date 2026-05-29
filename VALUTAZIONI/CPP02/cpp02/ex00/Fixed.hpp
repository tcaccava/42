/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:22:04 by mla-mona          #+#    #+#             */
/*   Updated: 2024/08/29 19:07:34 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP

#include <string>
#include <iostream>

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
        
        int getRawBits(void) const;
        void setRawBits(int const raw);
};


#endif