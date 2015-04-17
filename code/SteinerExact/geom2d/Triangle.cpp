#include "Triangle.h"

#include <math.h>

namespace geom2d{

Triangle::Triangle(Point &p1, Point &p2, Point &p3): corners{p1,p2,p3}
{
    //corners[0] = p1;
    //corners[1] = p2;
    //corners[2] = p3;
}

double Triangle::height(Point &p1, Point &p2, Point &p3)
{
    return height(p2.distance(p3), p3.distance(p1), p1.distance(p2));
}

double Triangle::height(double a, double b, double c)
{
    return 2*area(a,b,c)/c;
}

double Triangle::area(double a, double b, double c)
{
    double s = (a+b+c)*0.5;
    return sqrt(s*(s-a)*(s-b)*(s-c));
}

}
