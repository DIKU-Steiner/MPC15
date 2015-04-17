#ifndef BRANCHER_H
#define BRANCHER_H

#include <vector>

#include "SmithStar/WSNode.h"

class Brancher{
public:
    virtual void branch(WSNode *n, std::vector<WSNode*> &children)=0;
    virtual WSNode* root()=0;
};

#endif // BRANCHER_H
