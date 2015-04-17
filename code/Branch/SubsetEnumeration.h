#ifndef SUBSETENUMERATION_H
#define SUBSETENUMERATION_H

#include <sys/types.h>
#include <stdint.h>
#include <vector>

using namespace std;

class SubsetEnumeration
{
public:
    SubsetEnumeration(int N);

    vector<uint64_t>& getSubsetsOfSize(int sz);
private:
    int N;
    void ensureEnumerated(int sz);
    vector< vector<uint64_t> > subsets;
};

#endif // SUBSETENUMERATION_H
