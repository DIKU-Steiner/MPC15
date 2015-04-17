#ifndef WSLOGGER_H
#define WSLOGGER_H

#include <vector>
#include <string>
#include <ctime>

#include "SmithStar/WSNode.h"

using namespace std;

class WSLogger
{
public:
    WSLogger();

    void startTiming();
    void stopTiming();
    void exploredNode(WSNode *n, bool fathomed);

    void print();

private:
    vector<double> lowerBounds[100] = {};
    long fathomed[100] = {};
    long explored[100] = {};
    long spaceSizes[100] = {};
    clock_t startTime, stopTime;

    long spaceSize(int N);
};

#endif // WSLOGGER_H
