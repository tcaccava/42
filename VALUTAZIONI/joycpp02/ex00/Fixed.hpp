/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:49:37 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/16 14:49:40 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP

#include <iostream>

class Fixed {
private:
    int fixed;
    const static int bits = 8;
public:
    Fixed();
    Fixed(const Fixed &instance);
    Fixed& operator=(const Fixed &instance);
    ~Fixed();

    int getRawBits( void ) const;
    void setRawBits( int const raw );
};

#endif