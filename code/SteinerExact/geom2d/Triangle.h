#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <geom2d/Point2D.h>

namespace geom2d{

class Triangle
{
public:
    Triangle(Point &p1, Point &p2, Point &p3);

    /** Calculate the height from p3 */
    static double height(Point &p1, Point &p2, Point &p3);

    /** Calculate the height from the corner opposite the side with length c. */
    static double height(double a, double b, double c);

    /** Calculate the area based on the side-lengths. */
    static double area(double a, double b, double c);

private:
    Point corners[3];
};

}
#endif // TRIANGLE_H
