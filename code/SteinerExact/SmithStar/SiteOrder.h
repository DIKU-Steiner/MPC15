#ifndef SITEORDER_H
#define SITEORDER_H

#include <vector>

#include "SmithStar/Brancher.h"
#include "SmithStar/WSNode.h"
#include "geomNd/PointND.h"

using namespace std;

class SiteOrder: public Brancher
{
public:
    SiteOrder(vector< geomNd::Point > &sites);

    void branch(WSNode *n, vector<WSNode*> &children);
    WSNode* root();

private:
    int N;
};

#endif // SITEORDER_H
