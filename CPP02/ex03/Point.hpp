#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <string>
#include <cmath>
#include "Fixed.hpp"

class Point
{
public:
    Point();
    Point(const Fixed a, const Fixed b);
    Point(const Point &other);
    Point &operator=(const Point &other);
    ~Point();
    Fixed getX() const;
    Fixed getY() const;

private:
    Fixed const x;
    Fixed const y;
};

bool bsp(Point const a, Point const b, Point const c, Point const point);

#endif