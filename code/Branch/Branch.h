#ifndef GENGEOSTEINER_H
#define GENGEOSTEINER_H

#include <vector>
#include <list>
#include <utility>
#include <sys/types.h>
#include <stdint.h>

#include "geomNd/PointND.h"
#include "Branch/Topology.h"
#include "PointPlacement.h"
#include "ArgParser.h"

using namespace std;

//typedef unsigned int uint;
//typedef unsigned long long ulong;

#define MAX_N 32

class Branch
{
public:
    Branch(vector< geomNd::Point > &sites, double upperBound, ArgParser &argParser);

    /**
     * Goes through all terminals to check if any of them are contained in the interior of the
     * lens between p1 and p2
     * @param p1 Index of terminal 1
     * @param p2 Index of terminal 2
     * @return true iff there is no terminal in lens(p1, p2)
     */
    bool lensCheck(uint p1, uint p2);

    /**
     * Checks the two edges of the cherry to see if they will violate the bottleneck criterion
     * when concatenated with a terminal
     * @return true iff there exists a configuration of the cherry concatenated with the terminal
     * such that the longest edge is no longer than the bottleneck distance.
     */
    bool lensCheck(Topology &cherry, Topology &term);
    bool overlapCheck(vector<uint> &sites1, vector<uint> &sites2);
    bool bottleneckCheck(vector<uint> &sites1, vector<uint> &depths1, vector<uint> &sites2, vector<uint> &depths2);

    bool lowerboundCheck(Topology &top);

    void printTopologies();
    void printTopologies(uint lvl);
	void printLBEvals();

private:
    bool enableBottleneckCheck = true;
    bool enableTripleCheck = true;
    bool enableLowerboundCheck = true;
    bool enablePreprocessing = true;
    bool enableLowerboundHashing = true;

    void parseSubsets();

    const vector< geomNd::Point > sites;

    const uint N;///< Number of terminals
    const uint D;///< Number of terminals

	vector< int > lbLevels;

    vector< vector<double> > longestMSTDists;
    vector< vector<uint> > bottleneckDists;
    uint computeBottleneckDists();

    void preprocessLowerbounds(int maxWS, vector< pair<bitset<MAX_N>, double> > &subsets, vector< vector< pair<bitset<MAX_N>, double> > > &preprocessed, double upperBound);

    /** Enumerated topologies. The k-th level contains all permissible topologies containing
        k+1 terminals. The i-th elements of each vector represents a partial topology by
        referencing two indices in the previous vector. The 0-th entry is special and
        references terminals directly. */
    vector< list< Topology > > topologies;

    /** Goes through all topologies and removes all whose lowerBound exceeds the upperBound. Uses the
     * subsets to tighten the lowerBound. */
    void cleanupTopologies(double upperBound, vector< pair<bitset<MAX_N>, double> > &subsets);

    PointPlacement placement;
    double lowerBound(Topology & top, double tolerance);
    int setAdj(Topology &top, int p, int &c);

    void collectSites(Topology &top, uint depth, vector<uint> &sites, vector<uint> &depths);

    void printTop(Topology &top);
};

#endif // GENGEOSTEINER_H
