/*
 * floating_point_compare.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-29
 *      Author: Rick Valenzano
 */

#include "floating_point_utils.h"

bool fp_less(double a, double b)
{
    return (a < b - TOLERANCE);
}

bool fp_greater(double a, double b)
{
    return (a > b + TOLERANCE);
}

bool fp_equal(double a, double b)
{
    return !fp_less(a, b) && !fp_greater(a, b);
}

double fp_min(double a, double b)
{
    if(fp_greater(a,b))
        return b;
    return a;
}

double fp_max(double a, double b)
{
    if(fp_greater(b,a))
        return b;
    return a;
}


