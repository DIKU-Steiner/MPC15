#include "SiteOrder.h"

#include <list>
#include <algorithm>

SiteOrder::SiteOrder(vector< geomNd::Point > &sites):
    N(sites.size())
{
}

void SiteOrder::branch(WSNode *n, vector<WSNode*> &children)
{
    if(n->depth<2) { children.push_back(new WSNode( n, -1, n->depth )); return; }
    if(n->depth==N-1) return;

    int existingEdges = 2*n->depth-1;

    for(int e=0; e<existingEdges; e++){
        children.push_back(new WSNode(n,e, n->depth+1));
    }
}

WSNode* SiteOrder::root()
{
    WSNode *n = new WSNode(0);
    n = new WSNode(n, -1, 1);
    n = new WSNode(n, -1, 2);
    return n;
}
