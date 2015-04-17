#include "PartialSMT.h"

#include "SmithStar/WSNode.h"

PartialSMT::PartialSMT(vector< geomNd::Point > sites_, double tol):
    sites(sites_),
    tolerance(tol),
    placement(sites_),
    N(sites_.size()),
    edges(vector< vector<unsigned int> >( N+N-3, vector<unsigned int>(2) ))
{}

double PartialSMT::lowerBound(WSNode *node)
{

    node->updatePlacementAdjacencies(placement);

    //Update steiner point coordinates and return length lower bound
    return placement.getLowerbound(tolerance);

}
double PartialSMT::getLength(WSNode *node)
{
    node->updatePlacementAdjacencies(placement);
    return placement.getLength(tolerance);

}

void PartialSMT::printDeg()
{

}
