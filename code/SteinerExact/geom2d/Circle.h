#ifndef CIRCLE_H
#define CIRCLE_H

#include "geom2d/Point2D.h"

namespace geom2d{

class Point;

class Circle
{
public:
    Circle(Point &c, double r);
    ~Circle();

    void setRadius(double r){ radius = r; }
    void setCenter(Point &c);

    /** Return the intersection point between this and c such that
     * signbit(direction) == signbit(Point::turn( this->center(), c.center(), this->intersection(c, direction) )) */
    Point intersection(Circle &c, double direction);

private:
    Point *center;
    double radius;

};

}

#endif // CIRCLE_H
