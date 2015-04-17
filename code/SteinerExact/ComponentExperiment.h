#ifndef COMPONENTEXPERIMENT_H
#define COMPONENTEXPERIMENT_H

#include "PointPlacement.h"

#include "geomNd/PointND.h"

#include <vector>

class ComponentExperiment
{
public:
    ComponentExperiment();
    static void runWOPruning();
    static void runWPruning();

    static bool prunedByBottleneck(PointPlacement& placement, std::vector< std::vector<double> >& bottleneck_dists);
    static bool prunedByLune(PointPlacement& placement, std::vector< geomNd::Point >& points);
    static bool isDeg(PointPlacement& placement);
private:

    static void collectTerminals(PointPlacement& placement, int root, int rootParent, std::vector<int>& ret);
    static std::vector< std::vector<double> > steinerTreeBottleneckDistances(PointPlacement& placement);
    static std::vector< std::vector<double> > mstBottleneckDistances(std::vector<geomNd::Point>& points);
};

#endif // COMPONENTEXPERIMENT_H
