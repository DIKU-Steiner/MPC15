#include "TestCircle.h"

#include <iostream>
#include <math.h>

#include "geom2d/Circle.h"
#include "geom2d/Point2D.h"

using namespace geom2d;

bool TestCircle::runTest()
{
    if(testIntersection()){ std::cout<<"TestCircle .. testIntersection passed\n";
    } else { std::cout<<"TestCircle .. testIntersection failed\n"; return false; }

    return true;
}

bool TestCircle::testIntersection()
{

    Point p1(1,1);
    Point p2(3,1);
    Circle c1(p1, 1.0);
    Circle c2(p2, 1.0);
    Point i = c1.intersection(c2, 1);
    if(!i.equals( *(new Point(2,1)) )) return false;

    c2.setRadius(sqrt(5));
    i = c1.intersection(c2, 1);
    if(!i.equals( *(new Point(1,2)) )) return false;

    i = c1.intersection(c2, -1);
    if(!i.equals( *(new Point(1,0)) )) return false;

    c2.setRadius(3);
    i = c1.intersection(c2, -1);
    if(!i.equals( *(new Point(0,1)) )) return false;

    c1.setRadius(3);
    c2.setRadius(1);
    i = c1.intersection(c2, -1);
    if(!i.equals( *(new Point(4,1)) )) return false;

    return true;
}

