#ifndef SPANNINGTREERELAX_H
#define SPANNINGTREERELAX_H

#include <vector>

#include "PointPlacement.h"
#include "geomNd/PointND.h"

using namespace std;

class SpanningTreeRelax
{
public:
    SpanningTreeRelax();

    static PointPlacement topologyFromMST(vector<geomNd::Point> &sites);
private:
    static void addSteinerPoint(int, vector< vector<int> >&);
};

#endif // SPANNINGTREERELAX_H
