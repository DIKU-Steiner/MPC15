#ifndef POINT2D_H
#define POINT2D_H

#include <vector>
#include <iostream>

#include "geom2d/Circle.h"

namespace geom2d{

class Circle;

class Point
{
public:
    Point(double x, double y);
    Point(std::vector<double> &coords);

    double distance(Point &p);

    /** Returns true if this point has identical coordinates as p (to within a factor e). */
    bool equals(Point &p, double e=0.000001);

    /** Returns a positive number if p1->p2->p3 makes a left-turn, 0 if they're collinear, and a
     * negative number if they make a right-turn */
    static double turn(Point &p1, Point &p2, Point &p3);

    /** Finds the equilateral point of p1 and p2 in the direction specified by direction
     * (such that direction == turn(p1,p2,equilateralPoint(p1,p2,direction)) ) */
    static Point equilateralPoint(Point &p1, Point &p2, double direction);


    static Circle equilateralCircle(Point &p1, Point &p2, double direction);


    double x() const{ return coords[0]; }
    double y() const{ return coords[1]; }
private:
    double coords[2];
};

std::ostream& operator<<(std::ostream& out, const Point& p);

}

#endif // POINT2D_H
