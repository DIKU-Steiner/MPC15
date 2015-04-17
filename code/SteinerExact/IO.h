#ifndef IO_H
#define IO_H

#include <vector>

#include "geomNd/PointND.h"

using namespace std;

class IO
{
public:
    IO();
    static void readSTPFromFile(const char *fName, vector< geomNd::Point > &sites);
    static void readFromRandSphere(const int D, const int N, vector< geomNd::Point > &sites);
};

#endif // IO_H
