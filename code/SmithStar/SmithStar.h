#ifndef WARRENSMITHBNB_H
#define WARRENSMITHBNB_H

#include <vector>

#include "SmithStar/NodeComparator.h"
#include "SmithStar/Brancher.h"
#include "SmithStar/LowerBound.h"
#include "SmithStar/WSLogger.h"
#include "SmithStar/WSNode.h"
#include "geomNd/PointND.h"
#include "ArgParser.h"

using namespace std;

class SmithStar
{
public:
    //SmithStar(LowerBound *lb, Brancher *br, NodeComparator *pr);
    SmithStar(vector< geomNd::Point > &sites, ArgParser& argParser);
    SmithStar(vector< geomNd::Point > &sites);
    ~SmithStar();

    WSNode* solve(int N, double upperBound);

    void enableLogger();
    void printDeg();

    WSLogger logger;
private:
    LowerBound* lowerBound;
    Brancher* brancher;
    NodeComparator* prioritizer;
    bool loggerEnabled;
	double stopAfter;
};

#endif // WARRENSMITHBNB_H
