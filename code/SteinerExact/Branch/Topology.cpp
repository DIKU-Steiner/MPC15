#include "Topology.h"
#include <math.h>
#include <string.h>
#include <algorithm>

#define LONG1 ((uint64_t)1)
#define LONG3 ((uint64_t)3)

Topology::Topology(const Topology &other):
	left(other.left),
	right(other.right),
	minDepth(other.minDepth),
	siteMask(other.siteMask),
	lb(other.lb)
{

}

Topology::Topology(uint64_t i):
    left(0), right(0),
    minDepth(0),
    //leftDepth(0),
    //eulerBits(0),
    //leaves(1),
    siteMask(1L<<i),
    lb(0)
{
    //euler_normal = 0;
    //euler_left = 0;
    //leaf_order = i;
}

Topology::Topology(Topology &left_, Topology &right_):
    left(&left_), right(&right_),
    minDepth(std::min(left_.minDepth,right_.minDepth)+1),
    //leftDepth(left_.leftDepth+1),
    //eulerBits(left_.eulerBits+right_.eulerBits+4),
    //leaves(left_.leaves+right_.leaves),
    siteMask(left_.siteMask | right_.siteMask),
    lb(left_.lb+right_.lb)
{
    //euler_normal  = LONG1<<(eulerBits-1);
    //euler_normal |= left->euler_normal<<(right->eulerBits+3);
    //euler_normal |= LONG1<<(right->eulerBits+1);
    //euler_normal |= (right->euler_normal<<1);

    //euler_left  = (left->euler_left >> leftDepth) << (leftDepth+right->eulerBits+3+1);
    //euler_left |= (LONG3 << (leftDepth+right->eulerBits+1));
    //euler_left |= (right->euler_normal << (leftDepth+1));

    //leaf_order = left_.leaf_order<<(right_.leaves*5);
    //leaf_order|= right_.leaf_order;
}

int Topology::terminalIndex()
{
    return ffsl(siteMask)-1;
    //for(int i=0;i<siteMask.size();i++)
    //    if(siteMask.test(i)) return i;
}

//uint64_t Topology::euler_contracted(){
//    //uint64_t leftMask = (LONG1<<eulerBits)-1;
//    //leftMask ^= (LONG1<<(right->eulerBits+minDepth+2))-1;
//    //uint64_t rightMask = (LONG1<<(right->eulerBits+minDepth+1))-1;
//    //return ((euler_left&leftMask)>>2) | ((euler_left&rightMask)>>1);
//    uint64_t ret = 0;
//    ret  = (left->euler_left >> minDepth) << (minDepth+right->eulerBits+1+1);
//    ret |= (1L << minDepth+right->eulerBits);
//    ret |= (right->euler_normal << minDepth);
//    return ret;
//}
//
//uint64_t Topology::hash_value(){
//    return (siteMask<<44) | euler_contracted();
//
//}
