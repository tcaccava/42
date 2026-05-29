/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 14:15:54 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/09 15:07:19 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Point.hpp"
#include <cmath>

Fixed	getTriangleArea(Point const& a, Point const& b, Point const& c) {
	Fixed	area;
	Fixed	fact1;
	Fixed	fact2;
	Fixed	fact3;

	fact1 = a.getFixedX() * (b.getFixedY() - c.getFixedY());
	fact2 = b.getFixedX() * (c.getFixedY() - a.getFixedY());
	fact3 = c.getFixedX() * (a.getFixedY() - b.getFixedY());

	area = fact1 + fact2 + fact3;

	if (area < Fixed(0)) {
		area = area * Fixed(-1);
	}
	area = area * Fixed(0.5f);
	return (area);
}

bool bsp( Point const a, Point const b, Point const c, Point const point ) {
	Fixed		areaTot;
	Fixed		area1;
	Fixed		area2;
	Fixed		area3;
	const float	epsilon = 0.0001f;

	areaTot = getTriangleArea(a, b, c);
	area1 = getTriangleArea(a, b, point);
	area2 = getTriangleArea(b, c, point);
	area3 = getTriangleArea(c, a, point);

	if (((area1 + area2 + area3 - areaTot).toFloat() > epsilon)
		|| area1 <= 0 || area2 <= 0 || area3 <= 0) {
			return (false);
		}
	return (true);
}
