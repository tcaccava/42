/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Point.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:54:39 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/22 12:30:47 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINT_HPP
#define POINT_HPP

#include "Fixed.hpp"
#include <iostream>

class Point {
private:
    const Fixed x;
    const Fixed y;
public:
    Point();
    Point(Fixed x, Fixed y);
    Point(float x, float y);
    Point(const Point &instance);
    Point& operator=(const Point &instance);
    ~Point();
    
    Fixed get_X() const;
    Fixed get_Y() const;
};

std::ostream &operator << (std::ostream &out, const Point &point);

//bool bsp(Point const a, Point const b, Point const c, Point const point);
bool bsp_float(Point const a, Point const b, Point const c, Point const point);
bool bsp_fixed(Point const a, Point const b, Point const c, Point const point);

#endif