#include "SpanningTreeRelax.h"

#include <algorithm> //find

#include "MST.h"



SpanningTreeRelax::SpanningTreeRelax()
{
}

PointPlacement SpanningTreeRelax::topologyFromMST(std::vector<geomNd::Point> &sites)
{
    MST mst(sites);

    int N = sites.size();
    vector<vector<int>> adj;
    for(size_t i=0;i<N;i++)
        adj.push_back(mst.getAdjacencies(i));

    for(size_t i=0;i<N;i++){
        while(adj[i].size()>1)
            addSteinerPoint(i, adj);
    }

    PointPlacement placement(sites);
    for(int i=0;i<N-2;i++){
        placement.adj[i][0] = adj[N+i][0];
        placement.adj[i][1] = adj[N+i][1];
        placement.adj[i][2] = adj[N+i][2];
    }
    placement.steinerPoints = N-2;
    placement.getLength(0.0001);

    return placement;
}

void SpanningTreeRelax::addSteinerPoint(int vertex, vector< vector<int> > &adjacencies)
{
    int nextSteiner = adjacencies.size();

    int adj1 = adjacencies[vertex][0]; adjacencies[vertex].erase(adjacencies[vertex].begin());
    int adj2 = adjacencies[vertex][0]; adjacencies[vertex].erase(adjacencies[vertex].begin());
    adjacencies[vertex].push_back(nextSteiner);
    adjacencies[adj1].erase( find(adjacencies[adj1].begin(), adjacencies[adj1].end(), vertex) );
    adjacencies[adj2].erase( find(adjacencies[adj2].begin(), adjacencies[adj2].end(), vertex) );
    adjacencies[adj1].push_back( nextSteiner );
    adjacencies[adj2].push_back( nextSteiner );

    adjacencies.push_back(vector<int>());
    adjacencies[nextSteiner].push_back(vertex);
    adjacencies[nextSteiner].push_back(adj1);
    adjacencies[nextSteiner].push_back(adj2);
}
