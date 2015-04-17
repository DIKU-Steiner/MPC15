#include "NodeComparator.h"

#include <iostream>

NodeComparator::NodeComparator(const int method_): method(method_)
{}

bool NodeComparator::operator() (WSNode* lhs, WSNode* rhs) const
{
    switch(method){
    case BEST_FIRST: return lhs->lowerBound > rhs->lowerBound;
    case WORST_FIRST: return lhs->lowerBound < rhs->lowerBound;
    case DEPTH_FIRST: return lhs->depth < rhs->depth;
    case BREADTH_FIRST: return lhs->depth > rhs->depth;
    };

    std::cerr<<"NodeComparator error: unknown comparison method "<<method<<std::endl;
    throw "NodeComparator error: unknown comparison method";
}
