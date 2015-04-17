#include "PointND.h"

#include <math.h>
#include <fstream>

namespace geomNd{

Point::Point(int D): coords(D, 0)
{}

Point::Point(vector<double> &coords_): coords(coords_)
{}

int Point::dimensions() const
{ return coords.size(); }

double Point::distance(const Point &p) const
{
    double sqSum = 0;
    for(unsigned int d=0;d<coords.size();d++){
        double cDiff = coords[d]-p.coords[d];
        sqSum+=cDiff*cDiff;
    }
    return sqrt(sqSum);
}

double Point::angle(const Point &p1, const Point &p2, const Point &p3)
{
    double dotProduct = 0, len12 = 0, len32 = 0;
    for(size_t d=0;d<p1.coords.size(); d++){
        double diff12 = p1[d]-p2[d];
        double diff32 = p3[d]-p2[d];
        dotProduct += diff12*diff32;
        len12+=diff12*diff12;
        len32+=diff32*diff32;
    }
    //len12 = sqrt(len12);
    //len32 = sqrt(len32);
    return acos( dotProduct/sqrt(len12*len32) );
}

//inline const double& Point::operator[] (int idx) const
//{
//    return coords[idx];
//}
//
//inline double& Point::operator[] (int idx)
//{
//    return coords[idx];
//}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
    os<<"Point(";
    os.precision(8);
    for(int d=0;d<p.dimensions()-1; d++)
        os<<p[d]<<", ";
    os<<p[p.dimensions()-1]<<")";
    return os;
}

}
