#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <iostream>
#include <stdint.h>
#include <bitset>

class Topology
{
public:
    Topology *left, *right;
    //uint32_t minDepth, leftDepth, eulerBits, leaves;
    uint32_t minDepth;
    uint64_t siteMask;

    /** A bit-encoding of the Euler tour of this topology starting at the root */
    //uint64_t euler_normal;

    /** A bit-encoding of the Euler tour of this topology starting at the leftmost child */
    //uint64_t euler_left;

    //uint64_t leaf_order;

    double lb;
	Topology(const Topology &other);
    //Topology(uint l1, uint i1, uint l2, uint i2, uint minD, u_quad_t mask);
    Topology(Topology &left, Topology &right);
    Topology(uint64_t i);

    /** Assumes that this topology is a terminal (left==0 and siteMask only has one bit set) and returns
        the corresponding terminal index. */
    int terminalIndex();

    /** Return the euler tour of the tree that results from contracting the root-to-left-child-edge, starting
     * from the leftmost leaf */
    //uint64_t euler_contracted();
    //uint64_t hash_value();
};



#endif // TOPOLOGY_H
