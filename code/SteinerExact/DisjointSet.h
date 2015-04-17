#ifndef DISJOINTSET_H
#define DISJOINTSET_H

/**
 * @brief The DisjointSet class
 * From http://kartikkukreja.wordpress.com/2013/05/07/union-find-disjoint-set-data-structure-c-implementation/
 */
class DisjointSet
{
public:

    DisjointSet(int N);

    ~DisjointSet();

    /// Return the id of component corresponding to object p.
    int find(int p);

    /// Replace sets containing x and y with their union.
    void merge(int x, int y);

    /// Are objects x and y in the same set?
    bool connected(int x, int y);

    /// Return the number of disjoint sets.
    int count();

private:
    int *id, cnt, *sz;
};

#endif // DISJOINTSET_H
