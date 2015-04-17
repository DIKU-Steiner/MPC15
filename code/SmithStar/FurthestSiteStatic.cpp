#include "FurthestSiteStatic.h"

#include <list>
#include <algorithm>

FurthestSiteStatic::FurthestSiteStatic(vector< geomNd::Point > &sites):
    N(sites.size()),
    siteOrder(N)
{
    //Find three sites furthest apart and put them in siteOrder[0] to siteOrder[2]
    double maxDist = 0;
    for(int i=0;i<N;i++){
        for(int j=i+1;j<N;j++){
            double dij = sites[i].distance(sites[j]);
            for(int k=j+1;k<N;k++){
                double dik = sites[i].distance(sites[k]);
                double djk = sites[j].distance(sites[k]);
                if(dij+dik+djk>maxDist){
                    maxDist = dij+dik+djk;
                    siteOrder[0] = i;
                    siteOrder[1] = j;
                    siteOrder[2] = k;
                }
            }
        }
    }

    //Iteratively add the remaining sites starting with the furthest site
    list<int> added;
    list<int> remaining;
    for(int i=0;i<N;i++) remaining.push_back(i);
    for(int i=0;i<3;i++) {
        added.push_back(siteOrder[i]);
        remaining.erase(find(remaining.begin(),remaining.end(),siteOrder[i]));
    }

    for(int i=3;i<N;i++) {
        //Find the remaining that is furthest from the added
        double maxDist = 0;
        int maxR = -1;
        for(auto rIt=remaining.begin();rIt!=remaining.end();++rIt){//for(Integer r: remaining){
            for(auto aIt=added.begin();aIt!=added.end();++aIt){//for(Integer a: added){
                double dist = sites[*rIt].distance(sites[*aIt]);
                if(dist>maxDist){
                    maxDist = dist;
                    maxR = *rIt;
                }
            }
        }
        //Add it to added and remove from remaining
        added.push_back(maxR);
        remaining.erase(find(remaining.begin(),remaining.end(),maxR));
        siteOrder[i] = maxR;
    }

}

void FurthestSiteStatic::branch(WSNode *n, vector<WSNode*> &children)
{
    if(n->depth<2) { children.push_back(new WSNode( n, -1, siteOrder[n->depth] )); return; }
    if(n->depth==N-1) return;

    int existingEdges = 2*n->depth-1;

    for(int e=0; e<existingEdges; e++){
        children.push_back(new WSNode(n,e, siteOrder[n->depth+1]));
    }
}

WSNode* FurthestSiteStatic::root()
{
    WSNode *n = new WSNode(siteOrder[0]);
    n = new WSNode(n, -1, siteOrder[1]);
    n = new WSNode(n, -1, siteOrder[2]);
    return n;
}
