/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 12:02:06 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/06 14:34:46 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_POINT_HPP
#define FIXED_POINT_HPP

class Fixed {

private:
	int					rawBits;
	static const int	fractionalBits = 8;

public:
	Fixed();
	Fixed( const Fixed& Fixed );

	Fixed& operator=( const Fixed& Fixed );

	int		getRawBits( void ) const;
	void	setRawBits( int const raw );

	~Fixed();
};

#endif
