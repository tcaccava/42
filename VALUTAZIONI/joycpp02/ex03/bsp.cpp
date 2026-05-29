/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:53:37 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/22 10:45:51 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Fixed.hpp"
#include "Point.hpp"

// with float
float calc_area_float(Point const A, Point const B, Point const C) {

    float Ax = A.get_X().toFloat();
    float Ay = A.get_Y().toFloat();
    
    float Bx = B.get_X().toFloat();
    float By = B.get_Y().toFloat();
    
    float Cx = C.get_X().toFloat();
    float Cy = C.get_Y().toFloat();
    
    float res = Ax * (By - Cy) + Bx * (Cy - Ay) + Cx * (Ay - By);
    if (res < 0)
        res = -res;
    return res / 2.0;
}

bool bsp_float(Point const a, Point const b, Point const c, Point const point)
{ 
    float ABC = calc_area_float(a, b, c);

    float ABP = calc_area_float(a, b, point);
    float ACP = calc_area_float(a, c, point);
    float BCP = calc_area_float(b, c, point);

    return (ABP + ACP + BCP) == ABC;
}

// with fixed
Fixed calc_area_fixed(Point const A, Point const B, Point const C) {
    Fixed Ax = A.get_X();
    Fixed Ay = A.get_Y();
    Fixed Bx = B.get_X();
    Fixed By = B.get_Y();
    Fixed Cx = C.get_X();
    Fixed Cy = C.get_Y();

    Fixed res = Ax * (By - Cy) + Bx * (Cy - Ay) + Cx * (Ay - By);
    if (res < Fixed(0))
        res = res * Fixed(-1);
    return res / Fixed(2);
}

bool bsp_fixed(Point const a, Point const b, Point const c, Point const point)
{
    Fixed ABC = calc_area_fixed(a, b, c);
    Fixed ABP = calc_area_fixed(a, b, point);
    Fixed ACP = calc_area_fixed(a, c, point);
    Fixed BCP = calc_area_fixed(b, c, point);

    if (ABP == Fixed(0) || ACP == Fixed(0) || BCP == Fixed(0))
        return false;

    return (ABP + ACP + BCP) == ABC;
}