#include "Utils.h"

#include <math.h>

double Utils::dist(const vector<double> &p1, const vector<double> &p2){
    double sqSum = 0;
    for(size_t i=0;i<p1.size();i++){
        double diff = p1[i]-p2[i];
        sqSum += diff*diff;
    }
    return sqrt(sqSum);
}

double Utils::angle(const vector<double> &p1, const vector<double> &p2, const vector<double> &p3)
{
    vector<double> v1 = sub(p1,p2);
    vector<double> v2 = sub(p3,p2);
    return acos( dot(v1,v2)/(length(v1)*length(v2)) );
}

double Utils::length(vector<double> &v)
{
    return sqrt(dot(v,v));
}

double Utils::dot(vector<double> &v1, vector<double> &v2)
{
    double ret = 0;
    for(size_t i=0;i<v1.size();i++){
        ret += v1[i]*v2[i];
    }
    return ret;
}

vector<double> Utils::add(const vector<double> &p1, const vector<double> &p2)
{
    vector<double> ret(p1.size());
    for(size_t i=0;i<p1.size();i++){
        ret[i] = p1[i]+p2[i];
    }
    return ret;
}

vector<double> Utils::sub(const vector<double> &p1, const vector<double> &p2)
{
    vector<double> ret(p1.size());
    for(size_t i=0;i<p1.size();i++){
        ret[i] = p1[i]-p2[i];
    }
    return ret;
}

vector<double> Utils::mul(const vector<double> &p1, const double s)
{
    vector<double> ret(p1.size());
    for(size_t i=0;i<p1.size();i++){
        ret[i] = p1[i]*s;
    }
    return ret;
}

vector<double> Utils::div(const vector<double> &p1, const double s)
{
    vector<double> ret(p1.size());
    for(size_t i=0;i<p1.size();i++){
        ret[i] = p1[i]/s;
    }
    return ret;
}
