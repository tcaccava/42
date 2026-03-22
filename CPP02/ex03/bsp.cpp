#include "Fixed.hpp"
#include "Point.hpp"

Fixed area(Point const p1, Point const p2, Point const p3)
{
    Fixed res = ((p2.getX() - p1.getX()) * (p3.getY() - p1.getY())) -
                ((p2.getY() - p1.getY()) * (p3.getX() - p1.getX()));

    if (res < Fixed(0))
        res = res * Fixed(-1);
    return res / Fixed(2);
}

bool bsp(Point const a, Point const b, Point const c, Point const point)
{

    Fixed A = area(a, b, c);
    Fixed A1 = area(point, b, c);
    Fixed A2 = area(a, point, c);
    Fixed A3 = area(a, b, point);
    if (A1 == Fixed(0) || A2 == Fixed(0) || A3 == Fixed(0))
        return false;
    if (A1 + A2 + A3 == A)
        return true;
    return false;
}