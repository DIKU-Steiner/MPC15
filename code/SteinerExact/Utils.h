#ifndef UTILS_H
#define UTILS_H

#include <vector>

using namespace std;

class Utils
{
public:
    static double dist(const vector<double> &p1, const vector<double> &p2);
    static double angle(const vector<double> &p1, const vector<double> &p2, const vector<double> &p3);
    static double dot(vector<double> &v1, vector<double> &v2);
    static double length(vector<double> &v);
    static vector<double> add(const vector<double> &p1, const vector<double> &p2);
    static vector<double> sub(const vector<double> &p1, const vector<double> &p2);
    static vector<double> mul(const vector<double> &p1, const double s);
    static vector<double> div(const vector<double> &p1, const double s);
};

#endif // UTILS_H
