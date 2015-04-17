#include "SmithStar.h"

#include "WSNode.h"
#include <queue>
#include <vector>

#include "SmithStar/PartialSMT.h"
#include "SmithStar/FurthestSiteStatic.h"
#include "SmithStar/SiteOrder.h"
#include "SmithStar/NodeComparator.h"
#include "SmithStar/PartialSMT.h"

SmithStar::SmithStar(vector< geomNd::Point > &sites, ArgParser& argParser):
	lowerBound(NULL),
	brancher(NULL),
	prioritizer(NULL),
	loggerEnabled(false)
{
	double tolerance = 0.001;
	if(argParser.optionExists("-tolerance")) tolerance = argParser.getOption_double("-tolerance");

	lowerBound = new PartialSMT(sites,tolerance);
	brancher = new FurthestSiteStatic(sites);
	if(argParser.optionExists("-brancher") && argParser.getOption("-brancher")=="SiteOrder"){
		brancher = new SiteOrder(sites);
	}

	stopAfter = 12.0*60.0*60.0; // Standard stop time = 12hours
    if(argParser.optionExists("-stopAfter")) stopAfter = argParser.getOption_double("-stopAfter");

	prioritizer = new NodeComparator(NodeComparator::BEST_FIRST);
	if(argParser.optionExists("-nodeComparator") && argParser.getOption("-nodeComparator")==string("WorstFirst")){
		prioritizer = new NodeComparator(NodeComparator::WORST_FIRST);
	}
	if(argParser.optionExists("-nodeComparator") && argParser.getOption("-nodeComparator")==string("DepthFirst")){
		prioritizer = new NodeComparator(NodeComparator::DEPTH_FIRST);
	}
	if(argParser.optionExists("-nodeComparator") && argParser.getOption("-nodeComparator")==string("BreadthFirst")){
		prioritizer = new NodeComparator(NodeComparator::BREADTH_FIRST);
	}
}

SmithStar::SmithStar(vector< geomNd::Point > &sites):
	lowerBound(new PartialSMT(sites)),
	brancher(new FurthestSiteStatic(sites)),
	prioritizer(new NodeComparator(NodeComparator::BEST_FIRST)),
	loggerEnabled(false)
{
	stopAfter = 12.0*60.0*60.0; // Standard stop time = 12hours
}

SmithStar::~SmithStar()
{
	delete lowerBound;
	delete brancher;
	delete prioritizer;
}



WSNode* SmithStar::solve(int N, double upperBound)
{
	if(loggerEnabled)
		logger.startTiming();

	const clock_t startTime = clock();

	vector<WSNode*> children;
	WSNode* bestNode = NULL;
	priority_queue<WSNode*, vector<WSNode*>, NodeComparator> nodePool(*prioritizer);

	WSNode* root = brancher->root();
	//int[] f = brancher.firstThree();
	//WSNode* root = new WSNode(new WSNode(new WSNode(f[0]), -1, f[1]), -1, f[2]);
	root->lowerBound = lowerBound->lowerBound(root);
	if(N==3) return root;

	nodePool.push(root);

	while(!nodePool.empty()){
		WSNode *n = nodePool.top();
		nodePool.pop();
		if(loggerEnabled) logger.exploredNode(n, false);

		//Check if we need to stop
		double elapsedSecs = (double(clock()-startTime)/CLOCKS_PER_SEC);
		if(elapsedSecs>stopAfter){
			cout<<"SmithStar::StopAfter: "<<stopAfter<<endl;
			cout<<elapsedSecs<<"s elapsed. Stopping before finished."<<endl;
			throw "Ran out of time";
		}

		children.clear();
		brancher->branch(n,children);
		for(auto child_it = children.begin(); child_it!=children.end(); ++child_it){
			WSNode *child = *child_it;
			child->lowerBound = lowerBound->lowerBound(child);
			if(child->lowerBound<upperBound){
				if(child->depth+1==N){
					//Decrease tolerance before changing upper bound
					child->lowerBound = lowerBound->getLength(child);
					if(child->lowerBound>upperBound) {
						delete child;
						continue;
					}

					if(bestNode!=NULL)
						delete bestNode;
					bestNode = child;
					upperBound = child->lowerBound;
					if(loggerEnabled) {
						logger.exploredNode(child, false);
						cout<<"Improved solution: "<<upperBound<<endl;
					}
				}else{
					nodePool.push(child);
				}
			}else{
				if(loggerEnabled) logger.exploredNode(child, true);
				delete child;
			}
		}
	}

	if(loggerEnabled) logger.stopTiming();

	return bestNode;

}

void SmithStar::printDeg()
{
}

void SmithStar::enableLogger()
{
	loggerEnabled = true;
}
