#include "SubsetEnumeration.h"

SubsetEnumeration::SubsetEnumeration(int N_): N(N_)
{
}

//vector<uint64_t>& SubsetEnumeration::getSubsetsOfSize(int sz)
//{
//	
//}

void SubsetEnumeration::ensureEnumerated(int sz)
{
    if(sz<=subsets.size()) return;

    for(int i=subsets.size();i<sz;i++){
        subsets.push_back(vector<uint64_t>());


    }
}
