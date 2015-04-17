#include "Circle.h"

#include "geom2d/Triangle.h"
#include "geom2d/Point2D.h"
#include <math.h>

namespace geom2d{

Circle::Circle(Point &c, double r): center(new Point(c)), radius(r)
{}

Circle::~Circle()
{
    delete center;
}

void Circle::setCenter(Point &c){
    delete center;
    center = new Point(c);
}

Point Circle::intersection(Circle &c, double direction){
    int sgn = (direction>0) - (direction<0);
    double d = center->distance(*(c.center));
    double h = Triangle::height(radius, c.radius, d);
    //double dh = sqrt(radius*radius - h*h);
    double dh = (d*d-c.radius*c.radius+radius*radius)/(d+d);
    double diff[2] = { ( c.center->x()-center->x() )/d , ( c.center->y()-center->y() )/d  };
    return Point( center->x() + diff[0]*dh - diff[1]*h*sgn, center->y() + diff[1]*dh + diff[0]*h*sgn );
}

}
