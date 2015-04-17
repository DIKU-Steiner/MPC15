#ifndef FURTHESTSITESTATIC_H
#define FURTHESTSITESTATIC_H

#include <vector>

#include "SmithStar/Brancher.h"
#include "SmithStar/WSNode.h"
#include "geomNd/PointND.h"

using namespace std;

class FurthestSiteStatic: public Brancher
{
public:
    FurthestSiteStatic(vector< geomNd::Point > &sites);

    void branch(WSNode *n, vector<WSNode*> &children);
    WSNode* root();

private:
    int N;
    vector<int> siteOrder;
};

#endif // FURTHESTSITESTATIC_H
