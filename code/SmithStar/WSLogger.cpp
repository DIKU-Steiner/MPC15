#include "WSLogger.h"

#include <iostream>

WSLogger::WSLogger()
{
}

void WSLogger::startTiming()
{
    startTime = clock();
}

void WSLogger::stopTiming()
{
    stopTime = clock();
}

void WSLogger::exploredNode(WSNode *n, bool fathomed)
{
    lowerBounds[n->depth].push_back(n->lowerBound);
    if(fathomed) this->fathomed[n->depth]++;
    else 		this->explored[n->depth]++;

}

void WSLogger::print()
{
	int totalEval = 0;
	float avg = 0;
	for(int i=2;i<100;i++){
		if(lowerBounds[i].size()==0) continue;

		cout<<"Evaluated_Level: "<<(i+1)<<" "<<lowerBounds[i].size()<<"\n";
		totalEval+=lowerBounds[i].size();
		avg+=lowerBounds[i].size()*(i+1);
	}
	avg/=totalEval;

	cout<<"Evaluated_Nodes: "<<totalEval<<"\n";
	cout<<"Avg_Node_Size: "<<avg<<"\n";
}

long WSLogger::spaceSize(int N){
    if(N<4) return 1;

    long nom = 1;
    for(int i=N-1;i<=N+N-4;i++)
        nom*=i;
    long denom = 1;
    denom<<=(N-2);
    return nom/denom;
}
