/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Point.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 14:15:51 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 09:11:59 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Point.hpp"

Point::Point( ) : x(0.0f), y(0.0f) {

}

Point::Point( const float& xRaw, const float& yRaw ) : x(xRaw), y(yRaw) {

}

Point::Point( const Point& point ) : x(point.x), y(point.y) {

}

Point&	Point::operator=( const Point& point ) {
	(void) point;

	return (*this);
}

const Fixed&	Point::getFixedX( void ) const {
	return (x);
}

const Fixed&	Point::getFixedY( void ) const {
	return (y);
}

Point::~Point() {

}
