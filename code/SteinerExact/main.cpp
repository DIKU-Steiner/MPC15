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

#define DIMACS

using namespace std;

void printUsage(char* pName);

int runBranch(vector< geomNd::Point > &sites, double upperBound, ArgParser &argParser);
int runSmithStar(vector< geomNd::Point > &sites, double upperBound, ArgParser &argParser);

#include <math.h>


int main(int argc, char** argv)
{
    //ComponentExperiment::runWOPruning();
    //if(true) return 0;

    //bool swapIandK = true;
    //int N = 20;
    //int n2 = (int)floor(N/2.0);
    //int n3 = (int)ceil(N/3.0);
    //for(int i=n3;i<=n2;i++){
    //    int ni2 = (int)ceil( (N-i)/2.0 );
    //    for(int j=ni2;j<=min(i,N-i-1);j++){
    //        int k=N-i-j;
    //        if(swapIandK){ int tmp = i; i = k; k = tmp; }
    //        cout<<i<<" "<<j<<" "<<k<<endl;
    //        if(swapIandK){ int tmp = i; i = k; k = tmp; }
    //    }
    //}
    //if(true) return 0;

    //Topology* t1 = new Topology(0);
    //Topology* t2 = new Topology(1);
    //Topology* t3 = new Topology(2);
    //Topology* t4 = new Topology(3);
    //vector<Topology*> vec;
    //vec.push_back(t1);
    //vec.push_back(t2);
    //vec.push_back(t3);
    //vec.push_back(t4);
    //int c1 = 0;
    //for(auto it1 = vec.begin(); it1!=vec.end(); it1++){
    //    int c2 = 0;
    //    for(auto it2 = vec.begin(); it2!=vec.end(); it2++){
    //        cout<<c1<<" .. "<<c2<<endl;
    //        if(it1==it2){
    //            cout<<(*it1)->siteMask<<" = "<<(*it2)->siteMask<<endl;
    //            break;
    //        }
    //        c2++;
    //    }
    //    c1++;
    //}
    //if(true) return 0;


    if(sizeof(uint64_t)<8){
        cerr<<"To speed up lower bound computations this program uses bittricks that requires uint64_t to be a"<<endl;
        cerr<<"64 bit unsigned integer."<<endl;
        return -1;
    }

    ArgParser argParser(argc, argv);
    argParser.printArgs();
    if(argc==1 || argParser.optionExists("-h") || argParser.optionExists("-help")){
        printUsage(argv[0]);
        return -1;
    }
    //TestCircle::runTest();
    //return 0;

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

	//Only relevant with carioca_3_11_01.stp
	//PointPlacement placement(sites);
	//placement.points[11][0] = 0.480627;placement.points[11][1] = 0.590529;placement.points[11][2] = 0.646099;
	//placement.points[12][0] = 0.706996;placement.points[12][1] = 0.636315;placement.points[12][2] = 0.730917;
	//placement.points[13][0] = 0.465169;placement.points[13][1] = 0.512995;placement.points[13][2] = 0.566719;
	//placement.points[14][0] = 0.437907;placement.points[14][1] = 0.552696;placement.points[14][2] = 0.43286;
	//placement.points[15][0] = 0.376758;placement.points[15][1] = 0.740735;placement.points[15][2] = 0.294609;
	//placement.points[16][0] = 0.475153;placement.points[16][1] = 0.858765;placement.points[16][2] = 0.277051;
	//placement.points[17][0] = 0.485569;placement.points[17][1] = 0.143766;placement.points[17][2] = 0.655085;
	//placement.points[18][0] = 0.414695;placement.points[18][1] = 0.635309;placement.points[18][2] = 0.292104;
	//placement.points[19][0] = 0.832745;placement.points[19][1] = 0.574972;placement.points[19][2] = 0.657554;
	//placement.adj[0][0] = 13; placement.adj[0][1] = 12; placement.adj[0][2] = 2 ;
	//placement.adj[1][0] = 19; placement.adj[1][1] = 11; placement.adj[1][2] = 3 ;
	//placement.adj[2][0] = 17; placement.adj[2][1] = 11; placement.adj[2][2] = 14;
	//placement.adj[3][0] = 18; placement.adj[3][1] = 13; placement.adj[3][2] = 5 ;
	//placement.adj[4][0] = 4 ; placement.adj[4][1] = 18; placement.adj[4][2] = 16;
	//placement.adj[5][0] = 6 ; placement.adj[5][1] = 15; placement.adj[5][2] = 7 ;
	//placement.adj[6][0] = 0 ; placement.adj[6][1] = 13; placement.adj[6][2] = 8 ;
	//placement.adj[7][0] = 14; placement.adj[7][1] = 15; placement.adj[7][2] = 9;
	//placement.adj[8][0] = 1 ; placement.adj[8][1] = 12; placement.adj[8][2] = 10;
	//placement.steinerPoints = 9;
	//cout<<placement.lengthDebug()<<endl;
	//if(true)return 0;

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

    //PointPlacement placement = SpanningTreeRelax::topologyFromMST(sites);
    //cout<<placement<<endl;
    //double heu = placement.getLength(0.0001);
    //double mst = UB;
    //cout<<heu<<endl;
    //cout<<mst<<endl;
    //cout<<(heu/mst)<<endl;
    //return 0;


    //return runSmithStar(sites, UB, argParser);
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
    //ggs.printTopologies();

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

