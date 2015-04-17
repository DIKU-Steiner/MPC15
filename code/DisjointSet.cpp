#include "DisjointSet.h"

// Create an empty union find data structure with N isolated sets.
DisjointSet::DisjointSet(int N)   {
    cnt = N;
    id = new int[N];
    sz = new int[N];
    for(int i=0; i<N; i++)  {
        id[i] = i;
        sz[i] = 1;
    }
}

DisjointSet::~DisjointSet()       {
    delete [] id;
    delete [] sz;
}

// Return the id of component corresponding to object p.
int DisjointSet::find(int p)     {
    int root = p;
    while (root != id[root])
        root = id[root];
    while (p != root) {
        int newp = id[p];
        id[p] = root;
        p = newp;
    }
    return root;
}

// Replace sets containing x and y with their union.
void DisjointSet::merge(int x, int y)    {
    int i = find(x);
    int j = find(y);
    if (i == j) return;

    // make smaller root point to larger one
    if   (sz[i] < sz[j])    {
        id[i] = j;
        sz[j] += sz[i];
    } else  {
        id[j] = i;
        sz[i] += sz[j];
    }
    cnt--;
}

// Are objects x and y in the same set?
bool DisjointSet::connected(int x, int y)    {
    return find(x) == find(y);
}

// Return the number of disjoint sets.
int DisjointSet::count() {
    return cnt;
}
