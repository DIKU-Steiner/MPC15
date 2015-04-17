#ifndef POINTND_H
#define POINTND_H

#include <vector>
#include <iostream>

using namespace std;

namespace geomNd{

class Point
{
public:
    Point(int D);
    Point(vector<double> &coords);

    int dimensions() const;
    double distance(const Point &p) const;
    static double angle(const Point &p1, const Point &p2, const Point &p3);

    inline const double& operator[] (int idx) const{ return coords[idx]; }
    inline double& operator[] (int idx){ return coords[idx]; }

private:
    vector<double> coords;
};

ostream& operator<<(ostream& os, const Point& p);


}
#endif // POINTND_H
