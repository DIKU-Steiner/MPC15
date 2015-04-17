#include "Point2D.h"

#include <math.h>


namespace geom2d{

Point::Point(double x, double y)
{
    coords[0] = x;
    coords[1] = y;
}
Point::Point(std::vector<double> &coords)
{
    this->coords[0] = coords[0];
    this->coords[1] = coords[1];
}

double Point::distance(Point &p)
{
    double xDiff = coords[0]-p.coords[0];
    double yDiff = coords[1]-p.coords[1];
    return sqrt(xDiff*xDiff + yDiff*yDiff);
}

bool Point::equals(Point &p, double e)
{
    return fabs(coords[0]-p.coords[0])<e && fabs(coords[1]-p.coords[1])<e;
}

double Point::turn(Point &p1, Point &p2, Point &p3)
{
    double d1[2] = { p2.coords[0]-p1.coords[0], p2.coords[1]-p1.coords[1] };
    double d2[2] = { p3.coords[0]-p2.coords[0], p3.coords[1]-p2.coords[1] };
    return d1[0]*d2[1] - d1[1]*d2[0];
}

Point Point::equilateralPoint(Point &p1, Point &p2, double direction)
{
    double sgn = (direction>0) - (direction<0);
    double cos60 = 0.5;
    double sin60 = -sqrt(3)*0.5*sgn;
    double d12[2] = { p2.coords[0]-p1.coords[0], p2.coords[1]-p1.coords[1] };
    double x = ( d12[0]*cos60 + d12[1]*sin60) + p1.coords[0];
    double y = ( d12[1]*cos60 - d12[0]*sin60) + p1.coords[1];
    return Point(x,y);
}

Circle Point::equilateralCircle(Point &p1, Point &p2, double direction)
{
    Point eq = equilateralPoint(p1,p2,direction);
    Point center( (p1.coords[0]+p2.coords[0]+eq.coords[0])/3, (p1.coords[1]+p2.coords[1]+eq.coords[1])/3 );
    return Circle(center, eq.distance(p1));
}

std::ostream& operator<<(std::ostream& out, const Point& p)
{
    out<<"("<<p.x()<<","<<p.y()<<")";
    return out;
}

}
