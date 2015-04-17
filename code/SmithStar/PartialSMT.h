#ifndef PARTIALSMT_H
#define PARTIALSMT_H

#include <vector>

#include "SmithStar/LowerBound.h"
#include "geomNd/PointND.h"
#include "PointPlacement.h"

using namespace std;

class PartialSMT: public LowerBound
{
public:
    PartialSMT(vector< geomNd::Point > sites, double tol = 0.005);
    virtual double lowerBound(WSNode *node);
    virtual double getLength(WSNode *node);

    virtual void printDeg();

private:
    const vector< geomNd::Point > sites;
    const double tolerance;
    PointPlacement placement;

    const unsigned int N;
    vector< vector<unsigned int> > edges;
};

#endif // PARTIALSMT_H
