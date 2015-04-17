#ifndef NODECOMPARATOR_H
#define NODECOMPARATOR_H

#include "SmithStar/WSNode.h"

class NodeComparator{
public:
    NodeComparator(const int method);
    bool operator() (WSNode* lhs, WSNode* rhs) const;

    static const int BEST_FIRST = 0;
    static const int BREADTH_FIRST = 1;
    static const int DEPTH_FIRST = 2;
    static const int WORST_FIRST = 3;
    static const int HYBRID = 4;
private:
    const int method;
};

#endif // NODECOMPARATOR_H
