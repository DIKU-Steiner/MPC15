#ifndef LOWERBOUND_H
#define LOWERBOUND_H

#include "SmithStar/WSNode.h"

class LowerBound{
public:
	virtual ~LowerBound();
    virtual double lowerBound(WSNode *node) = 0;
    virtual double getLength(WSNode *node) = 0;
    virtual void printDeg() = 0;
};

#endif // LOWERBOUND_H
