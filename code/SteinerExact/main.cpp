#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>     /* srand, rand */


#include "ArgParser.h"
#include "MST.h"
#include "Utils.h"
#include "Branch/Branch.h"
#include "SmithStar/SmithStar.h"
#include "SmithStar/PartialSMT.h"
#include "SmithStar/NodeComparator.h"
#include "SmithStar/Brancher.h"
#include "SmithStar/FurthestSiteStatic.h"
#include "IO.h"
#include "geomNd/PointND.h"
#include "geom2d/Point2D.h"
#include "geom2d/tests/TestCircle.h"
#include "Heuristics/SpanningTreeRelax.h"
#include "Branch/Topology.h"

#include "ComponentExperiment.h"
#include "TestWSConvergence.h"

using namespace std;

void printUsage(char* pName);

int runBranch(vector< geomNd::Point > &sites, double upperBound, ArgParser &argParser);
int runSmithStar(vector< geomNd::Point > &sites, double upperBound, ArgParser &argParser);

#include <math.h>


int main(int argc, char** argv)
{

    //if(sizeof(uint64_t)<8){
    //    cerr<<"To speed up lower bound computations this program uses bittricks that requires uint64_t to be a"<<endl;
    //    cerr<<"64 bit unsigned integer."<<endl;
    //    return -1;
    //}

    ArgParser argParser(argc, argv);
    argParser.printArgs();
    if(argc==1 || argParser.optionExists("-h") || argParser.optionExists("-help")){
        printUsage(argv[0]);
        return -1;
    }

    //Initialize random
    if(argParser.optionExists("-seed")) srand(argParser.getOption_int("-seed"));
    else srand(0);

    vector< geomNd::Point > sites;
    if(argParser.optionExists("-input"))
        IO::readSTPFromFile(argParser.getOption("-input").c_str(), sites);
    else{
        const int D = argParser.optionExists("-d")?argParser.getOption_int("-d"):2;
        const int N = argParser.optionExists("-n")?argParser.getOption_int("-n"):10;
        IO::readFromRandSphere(D, N, sites);
    }

	//Compute upper bound
    double UB;
    cout<<"Using upper bound from ";
    if(!argParser.optionExists("-upperBound") || (argParser.optionExists("-upperBound") && argParser.getOption("-upperBound")==string("Heuristic")) ){
        PointPlacement placement = SpanningTreeRelax::topologyFromMST(sites);
        UB = placement.getLength(0.001)+0.001;
        cout<<"heuristic";
    }else if(argParser.optionExists("-upperBound") && argParser.getOption("-upperBound")==string("MST")){
        MST mst(sites);
        UB = mst.getLength()+0.001;
        cout<<"MST";
    }else{
        UB = argParser.getOption_double("-upperBound");
        cout<<"command line";
    }
    cout<<": "<<UB<<endl;

	//Start correct Steiner method
    if(argParser.optionExists("-method") && argParser.getOption("-method")=="SmithStar"){
        return runSmithStar(sites, UB, argParser);
    }else{
        return runBranch(sites, UB, argParser);
    }

}


void printUsage(char* pName)
{
    cout<<"Usage:"<<endl;
    //cout<<pName<<" [-method <Branch | SmithStar>] [-d <dim> -n <terminals> | -input <fName>.stp] [-upperBound <float>]"<<endl;
    cout<<pName<<" [option list]"<<endl;
    cout<<"Where [option list] can contain the following:"<<endl;
    cout<<"  -method <Branch | SmithStar>   Default: Branch"<<endl;
    cout<<"  -d <int>  If no input is specified random terminals with this dimension will be generated. Default: 3"<<endl;
    cout<<"  -n <int>  If no input is specified this many random terminals will be generated. Default: 10"<<endl;
    cout<<"  -input <string>.stp  STP-formatted input file"<<endl;
    cout<<"  -seed <int>  Seed to be used for randomization. Default: 0"<<endl;
    cout<<"  -upperBound <float | MST | Heuristic>  Indicates the upper bound that will be used. Default: Heuristic"<<endl;
	cout<<"Options specific to Branch:"<<endl;
    cout<<"  -subsets <int>:<int|all>[,<int:int|all>,...]  Number of terminals and number of topologies to preprocess. Default: 5:all"<<endl;
	cout<<"Options specific to SmithStar:"<<endl;
    cout<<"  -brancher <SiteOrder|Furthest>  Branching strategy. Include terminals in the order they are presented or take furthest first (Default). "<<endl;
    cout<<"  -nodeComparator <BestFirst|DepthFirst|BreadthFirst>  Prioritization of BnB nodes. Default: BestFirst. "<<endl;
    cout<<"The following example will use Branch to solve a set of 12 terminals randomly distributed in a 4D hypercube:"<<endl;
    cout<<pName<<" -method Branch -d 4 -n 12"<<endl;
}

int runBranch(vector< geomNd::Point > &sites, double upperBound, ArgParser &argParser){
    clock_t start = clock();
    Branch ggs(sites, upperBound, argParser);
    clock_t end = clock();

	ggs.printLBEvals();

    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    cout<<"Took "<<seconds<<" seconds"<<endl;
    return 0;
}

int runSmithStar(vector< geomNd::Point > &sites, double upperBound, ArgParser &argParser){
    SmithStar bnb(sites, argParser);
    bnb.enableLogger();
    clock_t start = clock();
    WSNode *n = bnb.solve(sites.size(), upperBound);
    clock_t end = clock();


    PointPlacement placement(sites);
    n->updatePlacementAdjacencies(placement);
	double len = placement.getLength(0.00001);
    for(size_t i=0;i<sites.size()-2;i++) {
        printf("> steiner[%lu]: ",i+sites.size());
		for(int d=0;d<placement.D; d++){
			printf("%.5f ",placement.points[i+sites.size()][d]);
		}
		cout<<endl;
	}
    for(size_t i=0;i<sites.size()-2;i++) {
        printf("> steiner[%lu] adjacencies: ",i+sites.size());
        for(int j=0;j<3;j++) printf("%3d",placement.adj[i][j]);
        cout<<endl;
    }
    printf("Length: %.7f\n",len);

    bnb.logger.print();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    cout<<"Took "<<seconds<<" seconds"<<endl;

    return 0;

}

