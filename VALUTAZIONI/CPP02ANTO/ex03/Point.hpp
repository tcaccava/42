/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Point.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 14:15:48 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 09:11:59 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINT_HPP
#define POINT_HPP

#include "Fixed.hpp"

class Point {

private:
	const Fixed	x;
	const Fixed	y;

public:
	Point();
	Point( const float& x, const float& y );
	Point( const Point& point );

	Point&	operator=( const Point& point);

	const Fixed&	getFixedX( void ) const;
	const Fixed&	getFixedY( void ) const;

	~Point();
};

bool bsp( Point const a, Point const b, Point const c, Point const point);

#endif
