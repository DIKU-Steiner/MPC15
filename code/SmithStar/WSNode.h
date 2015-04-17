#ifndef WSNODE_H
#define WSNODE_H

#include "PointPlacement.h"

class WSNode
{
public:
    WSNode* parent;
    const int depth;
    const int edgeSplit;
    const int siteInserted;
    double lowerBound;
    int childrenAlive;

    WSNode(WSNode *parent, const int splitEdge, const int siteInserted);
    WSNode(const int siteInserted);
    ~WSNode();

    /** Updates the field placement.adj so it reflects this node */
    void updatePlacementAdjacencies(PointPlacement& placement);

private:

};

#endif // WSNODE_H
