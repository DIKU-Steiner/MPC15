#ifndef MST_H
#define MST_H

#include <vector>

#include "geomNd/PointND.h"

struct Edge {
    int u, v;
    double weight;
    Edge(int u_, int v_, double w_): u(u_), v(v_), weight(w_){}
    int opposite(int i){ return i==u?v:u; }
};

/**
 * @brief The Minimum Spanning Tree class
 * The constructore builds the minimum spanning tree. Two operations are then permitted: Retrieving the total length
 * of the MST and retrieving the longest edge-length on the path between two vertices in the tree.
 *
 * Construction is inspired by http://kartikkukreja.wordpress.com/2013/05/25/kruskals-minimum-spanning-tree-algorithm/
 */
class MST
{
public:
    /** Builds minimum spanning tree using Kruskals algorithm */
    MST(const std::vector< geomNd::Point > &sites);

    ~MST();

    /** Total length of tree */
    double getLength(){ return length; }

    /** Longest edge between vertices v1 and v2 */
    double longestEdge(int v1, int v2);

    void printTree();

    //std::vector<Edge*> findPath(int v1, int v2);

    /** Get all vertices adjacent to v in the MST. */
    std::vector<int> getAdjacencies(int v);

private:
    /** Adjacency list. Because path-location is easier in a directed graph we don't just store adjacent
     * vertices but adjacent edges (whose direction are easily reversed). */
    std::vector< std::vector<Edge*> > adjacencies;

    /** All the edges also stored in adjacencies. Makes destruction sligthly faster and easier. */
    Edge** mstEdges;

    /** Cached tree length. */
    double length;

    /** Set i to be the root of the sub-tree obtained by removing the edge (parent, i) */
    void rootTree(int parent, int i);

    /** Return the parent-pointer of v */
    Edge* parent(int v);

};

#endif // MST_H
