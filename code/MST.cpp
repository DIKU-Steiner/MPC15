#include "MST.h"

#include <iostream>
#include <algorithm>
#include <stdio.h>


#include "geomNd/PointND.h"
#include "DisjointSet.h"
#include "Utils.h"

using namespace std;

bool mstEdgeComp(Edge *a, Edge *b)   {
    return a->weight < b->weight;
}

MST::MST(const vector< geomNd::Point > &sites):
    adjacencies(sites.size(), vector<Edge*>())
{
    //Use Kruskal to find MST of sites
    int V = sites.size();
    int E = V*(V-1)/2;
    Edge **edges = new Edge*[E];
    int c=0;
    for(int i=0;i<V;i++){
        for(int j=i+1;j<V;j++){
            edges[c++] = new Edge( i, j, sites[i].distance(sites[j]) );
            //edges[c++] = new Edge(i, j, Utils::dist(sites[i], sites[j]));
        }
    }
    sort(edges, edges + E, mstEdgeComp);

    DisjointSet uf(V);
    mstEdges = new Edge*[V-1];
    int N = 0;
    length = 0.0;
    for(int i=0; i<E && N<V-1; i++) {
        int u = edges[i]->u;
        int v = edges[i]->v;
        if(!uf.connected(u, v)) {
            uf.merge(u, v);
            mstEdges[N++] = edges[i];
            length += edges[i]->weight;
        }else{
            delete edges[i];
        }
    }

    delete [] edges;

    //printf("Total length of MST : %d\n", length);
    //printf("Edges in MST :\n");
    for(int i=0; i<N; i++){
        adjacencies[mstEdges[i]->u].push_back(mstEdges[i]);
        adjacencies[mstEdges[i]->v].push_back(mstEdges[i]);
    //    printf("%d %d %.3f\n", mstEdges[i]->u, mstEdges[i]->v, mstEdges[i]->weight);
    }
}

MST::~MST()
{
    for(unsigned int i=0;i<adjacencies.size()-1;i++)
        delete mstEdges[i];

    delete [] mstEdges;

}

double MST::longestEdge(int v1, int v2)
{
    if(v1==v2) return 0;

    rootTree(-1, v1);

    Edge* e2 = parent(v2);
    double maxWeight = e2->weight;
    while(e2->u!=v1){
        e2 = parent(e2->u);
        maxWeight = max(maxWeight, e2->weight);
    }

    return maxWeight;
}

vector<int> MST::getAdjacencies(int v)
{
    vector<int> ret(adjacencies[v].size(), -1);

    for(size_t i=0;i<adjacencies[v].size();i++){
        ret[i] = adjacencies[v][i]->v==v?adjacencies[v][i]->u:adjacencies[v][i]->v;
    }
    return ret;
}

void MST::printTree()
{
    printf("Edges in MST :\n");
    for(unsigned int i=0; i<adjacencies.size()-1; i++){
        printf("> %d %d %.3f\n", mstEdges[i]->u, mstEdges[i]->v, mstEdges[i]->weight);
    }

}

void MST::rootTree(int parent, int i)
{
    for(unsigned int a=0;a<adjacencies[i].size();a++){
        if(adjacencies[i][a]->u!=parent){
            if(adjacencies[i][a]->v==i){
                //Change edge direction
                adjacencies[i][a]->v = adjacencies[i][a]->u;
                adjacencies[i][a]->u = i;
            }
            //Recursively root subtrees
            rootTree(i, adjacencies[i][a]->v );
        }
    }
}

Edge* MST::parent(int i){
    for(unsigned int a=0;a<adjacencies[i].size();a++){
        if(adjacencies[i][a]->u!=i) return adjacencies[i][a];
    }
    return NULL;
}

//vector<Edge*> MST::findPath(int v1, int v2)
//{
//    vector<Edge*> ret;
//    if(v1==v2) return ret;
//
//    rootTree(-1, v1);
//
//    Edge* e2 = parent(v2);
//    ret.push_back(e2);
//    while(e2->u!=v1){
//        e2 = parent(e2->u);
//        ret.push_back(e2);
//    }
//
//    //printf("Path:\n");
//    //for(int i=0;i<ret.size();i++){
//    //    printf(" (%d %d)", ret[i]->u, ret[i]->v);
//    //}
//    //printf("\n");
//
//    return ret;
//}
