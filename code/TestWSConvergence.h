#ifndef TESTWSCONVERGENCE_H
#define TESTWSCONVERGENCE_H

#include <vector>
#include <iostream>
#include <fstream>

#include "geomNd/PointND.h"

class TestWSConvergence
{
public:
    const int D;
    const int N;

    TestWSConvergence(std::vector< geomNd::Point > sites);

    /** First N entries are the terminal coordinates. The remaining N-2 are steiner points */
    double points[98][10];
    //std::vector< geomNd::Point > points;

    /** Adjacency list for steiner points. Entry i is a vector of 3 indices (into the points-field) indicating
     * which terminals or steiner points are adjacent to steiner points i. */
    int adj[48][3];
    //std::vector< std::vector<int> > adj;
    int steinerPoints;

    /** Given the terminal coordinates in the first N entries of points and the steiner point adjacencies in
     * adj, compute the coordinates of steiner points and place them in the last N-2 entries of points. Return
     * the length of the tree. */
    double getLength(const double tol);
    double getLowerbound(const double tol);
    double lengthDebug();

    void runTest1();
private:

    //Max terminals is 50. Max dimension is 10

    double EL[48][3];
    double B[50][3];
    double C[50][10];
    int eqnstack[50];
    int leafQ[50];
    int val[50];

    //std::vector< std::vector<double> > EL;
    //std::vector< std::vector<double> > B;
    //std::vector< std::vector<double> > C;
    //std::vector<int> eqnstack;
    //std::vector<int> leafQ;
    //std::vector<int> val;

    double length();
    double error();
    void optimize(const double tol);
};

ostream& operator<<(ostream& out, const TestWSConvergence& p);

#endif // TESTWSCONVERGENCE_H
