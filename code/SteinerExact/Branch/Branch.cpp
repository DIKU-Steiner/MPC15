#include "Branch.h"

#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <stdint.h> //uint64_t
#include <utility> //pair, make_pair
#include <bitset> //bitset
#include <algorithm> //sort, max
#include <ctime> //time_t, time, ctime, clock
#include <climits> //INT_MAX


#include "MST.h"
#include "Utils.h"
#include "geom2d/Point2D.h"
#include "geom2d/Circle.h"
#include "geom2d/Triangle.h"

#include "SmithStar/SmithStar.h"

using namespace std;

/** For sorting a vector of pairs by second item */
struct sort_by_second {
    bool operator()(const pair<bitset<MAX_N>,double> &left, const std::pair<bitset<MAX_N>,double> &right) {
        return left.second > right.second;
    }
};

struct lb_sorter {
    bool operator()(const Topology &left, const Topology &right) {
        return left.lb<right.lb;
    }
};


Branch::Branch(vector< geomNd::Point > &sites_, double upperBound, ArgParser &argParser):
    sites(sites_),
    N(sites.size()),
    D(sites[0].dimensions()),
    longestMSTDists(N, vector<double>(N, 0)),
    bottleneckDists(N, vector<uint>(N, 0)),
    topologies(N, list< Topology >() ),
    placement(sites_),
    lbLevels(100,0)
{
	const clock_t startTime = clock();

    upperBound+=0.001; //In case the upper bound is the optimal length

    MST mst(sites);
    double mstLen = mst.getLength();
    cout<<"Upper bound ratio: "<<(upperBound/mstLen)<<endl;

	double tolerance = 0.05; 
	if(argParser.optionExists("-tolerance")) tolerance = argParser.getOption_double("-tolerance");
    cout<<"Tolerance: "<<(tolerance)<<endl;


    enableBottleneckCheck = true;
    enableTripleCheck = true;
    enableLowerboundCheck = true;
    //enableLowerboundHashing = false;
    if(argParser.optionExists("-enableBottleneckCheck") && argParser.getOption("-enableBottleneckCheck")=="false") enableBottleneckCheck = false;
    if(argParser.optionExists("-enableTripleCheck") && argParser.getOption("-enableTripleCheck")=="false") enableTripleCheck = false;
    if(argParser.optionExists("-enableLowerboundCheck") && argParser.getOption("-enableLowerboundCheck")=="false") enableLowerboundCheck = false;
    if(argParser.optionExists("-enablePreprocessing") && argParser.getOption("-enablePreprocessing")=="false") enablePreprocessing = false;
    //if(argParser.optionExists("-enableLowerboundHashing") && argParser.getOption("-enableLowerboundHashing")=="false") enableLowerboundHashing = false;

	double stopAfter = 12.0*60.0*60.0; // Standard stop time = 12hours
    if(argParser.optionExists("-stopAfter")) stopAfter = argParser.getOption_double("-stopAfter");

    time_t rawtime;
    time (&rawtime);
    printf ("Time-stamp: %s", ctime(&rawtime));

	double elapsedSecs = (double(clock()-startTime)/CLOCKS_PER_SEC);
    printf ("CPU-time elapsed (in seconds): %.3f\n", elapsedSecs);

    //Preprocess subset of terminals. 
    vector< pair<bitset<MAX_N>, double> > subsets;
    vector< vector< pair<bitset<MAX_N>, double> > > preprocessed;
    if(enablePreprocessing){
        //int maxWS = argParser.optionExists("-maxWS")?argParser.getOption_int("-maxWS"):8;
        int maxWS = argParser.optionExists("-maxWS")?argParser.getOption_int("-maxWS"):min((N+1)/2,(unsigned int)8);
        preprocessLowerbounds(maxWS, subsets, preprocessed, upperBound);
    }

    time (&rawtime);
    printf ("Time-stamp: %s", ctime(&rawtime));

	elapsedSecs = (double(clock()-startTime)/CLOCKS_PER_SEC);
    printf ("CPU-time elapsed (in seconds): %.3f\n", elapsedSecs);

    //Precompute max number of edges between each pair of vertices
    uint maxBottleneck = computeBottleneckDists();

    //Create branches corresponding to terminals
    for(uint i=0;i<N;i++) {
        topologies[0].push_back( Topology(i) );
    }

    //Used in generation phase for pruning with bottleneck distances
    vector<uint> sites1;
    vector<uint> depths1;
    vector<uint> sites2;
    vector<uint> depths2;

    //Used for caching lower bounds
    //std::unordered_map<uint64_t,std::unordered_map<uint64_t,double> > lbMap(2000000);
    //std::unordered_map<uint64_t,double> lbMap(2000000);

    //ofstream myfile;
    //myfile.open ("eqLBs.txt");
    //myfile<<"#Level lowerBound"<<endl;

    //Equilateral point generation phase
    cout<<"Generating equilateral points"<<endl;
    for(uint k=1;k<(N/2);k++){
        cout<<"Equilateral points with "<<(k+1)<<" terminals .. ";


        //To generate all topologies at level k, concatenate all at levels l and k-l-1
        for(uint l=0;l<=(k-1)/2;l++){
			elapsedSecs = (double(clock()-startTime)/CLOCKS_PER_SEC);
			if(elapsedSecs>stopAfter){
				cout<<"StopAfter: "<<stopAfter<<endl;
				cout<<elapsedSecs<<"s elapsed. Stopping before finished."<<endl;
				return;
			}

            for(auto b1=topologies[k-l-1].begin();b1!=topologies[k-l-1].end();++b1){

                sites1.clear(); depths1.clear();

                for(auto b2=topologies[l].begin(); b2!=topologies[l].end(); ++b2){
                    if(l==(k-l-1) && b2==b1) break;

                    //Dont concatenate if b1 and b2 contain the same terminal
                    if( b1->siteMask & b2->siteMask ) {
                        //if( topologies[k-l-1][b1].siteMask & topologies[l][b2].siteMask ) {
                        //cout<<"prune 1"<<endl;
                        continue;
                    }

                    //Dont concatenate if an edge on the path between two terminals becomes longer than bottleneck dist
                    if( enableBottleneckCheck && b1->minDepth+b2->minDepth+2<maxBottleneck )
                    {
                        //Collecting sites is costly, so delay it as long as possible
                        if(sites1.empty()) collectSites(*b1, 0, sites1, depths1);
                        sites2.clear(); depths2.clear();
                        collectSites(*b2, 0, sites2, depths2);
                        if(!bottleneckCheck(sites1, depths1, sites2, depths2)) {
                            //cout<<"prune 2"<<endl;
                            continue;
                        }
                    }

                    //For triples it is sometimes possible to make slightly stronger pruning based on bottleneck distance
                    if( enableTripleCheck && k==2 )
                    {
                        if( b2->left==0 && !lensCheck(*b1, *b2) )
                            continue;
                        if( b1->left==0 && !lensCheck(*b2, *b1) )
                            continue;
                    }

                    //Concatenate topologies
                    bool firstIsLeft = b1->terminalIndex()<b2->terminalIndex();
                    Topology newTop = firstIsLeft?Topology(*b1, *b2):Topology(*b2, *b1);


                    //Discard concatenation if lower bound exceeds upper bound
                    if(enableLowerboundCheck) {

                        //Locate largest preprocessed solution that don't overlap with newTop
                        size_t i;
                        for(i=0;i<subsets.size(); i++){
                            if( !(subsets[i].first.to_ulong() & newTop.siteMask) ) break;
                        }
                        //i is now the index of the preprocessed solution with largest SMT that is disjoint from newTop
                        double subsetVal = enablePreprocessing?subsets[i].second:0.0;

                        //size_t j;
                        //for(j=0;j<preprocessed[N-k-1].size();j++){
                        //    if( !(preprocessed[N-k-1][j].first.to_ulong() & newTop.siteMask) ) break;
                        //}
                        //double preprocessedVal = enablePreprocessing?preprocessed[N-k-1][j].second:0.0;

                        //if(subsetVal==preprocessedVal){
                        //    cout<<" > "<<preprocessed[N-k-1][j].first<<endl;
                        //    cout<<" > "<<subsets[i].first<<endl;
                        //    cout<<" > ";
                        //    bitset<MAX_N> tbs(newTop.siteMask);
                        //    cout<<tbs<<endl;

                        //    cout<<(subsetVal<preprocessedVal)<<" .. "<<subsetVal<<" .. "<<preprocessedVal<<endl;
                        //}

                        //cout<<"Checking ";printTop(newTop);cout<<": "<<newTop.lb<<" + "<<subsetVal<<" > "<<upperBound;
                        //cout<<" ; "<<subsets[i].first<<endl;

                        //Do a quick check before spending time on the point placement
                        if(newTop.lb+subsetVal>upperBound) {
                            continue;
                        }

                        //Spend time on point placement
                        newTop.lb = lowerBound(newTop,tolerance);

                        //Check if lower bound can be looked up in hash-map
                        //if(enableLowerboundHashing){
                        //    uint64_t hashVal = newTop.hash_value();
                        //    //if(lbMap[hashVal]==0){
                        //    if(lbMap[hashVal].empty() || lbMap[hashVal][newTop.leaf_order]==0.0){
                        //        lbMap[hashVal][newTop.leaf_order] = newTop.lb = lowerBound(newTop);
                        //	}else{
                        //        newTop.lb = lbMap[hashVal][newTop.leaf_order];
                        //	}
                        //}else{
                        //    newTop.lb = lowerBound(newTop,0.005)-0.005;
                        //}
                        //printTop(newTop);
                        //cout<<bitset<64>(hashVal)<<endl;
                        //myfile<<k<<" "<<newTop.lb<<" "<<(newTop.lb+subsets[i].second)<<endl;

                        if(newTop.lb+subsetVal-0.005>upperBound) {
                            continue;
                        }
                    }
                    //if(printHere) cout<<"Used"<<endl;
                    topologies[k].push_back(newTop);
                }
            }
        }

        //Sort all topologies at level k in ascending order by lower bound
        topologies[k].sort(lb_sorter());
        cout<<topologies[k].size()<<endl;

    }

    time (&rawtime);
    printf ("Time-stamp: %s", ctime(&rawtime));

	elapsedSecs = (double(clock()-startTime)/CLOCKS_PER_SEC);
    printf ("CPU-time elapsed (in seconds): %.3f\n", elapsedSecs);

    //Concatenation phase
    cout<<"Concatenating equilateral points .."<<endl;
    unsigned int count = 0;

#ifdef DIMACS
	cout<<"DIMACS SECTION Solutions"<<endl;
	Topology best(0);
#endif

    int n2 = (int)floor(N/2.0);
    int n3 = (int)ceil(N/3.0);
    for(int i=n2;i>=n3;i--){
        int ni2 = (int)ceil( (N-i)/2.0 );
        for(int j=min((unsigned int)i,N-i-1);j>=ni2;j--){
            int k=N-i-j;
            cout<<i<<" "<<j<<" "<<k<<endl;
            int lvlCount = 0;

            //Concatenate all tops with i, j and k terminals
            for(auto topi = topologies[i-1].begin(); topi!=topologies[i-1].end();++topi){

				elapsedSecs = (double(clock()-startTime)/CLOCKS_PER_SEC);
				if(elapsedSecs>stopAfter){
				cout<<"StopAfter: "<<stopAfter<<endl;
					cout<<elapsedSecs<<"s elapsed. Stopping before finished."<<endl;
					return;
				}


                for(auto topj = topologies[j-1].begin(); topj!=topologies[j-1].end();++topj){
                    if(topj==topi) break;
                    if(topi->siteMask & topj->siteMask) continue;

                    Topology fullLeft(*topi, *topj);
                    if(fullLeft.lb>upperBound) continue;

                    size_t s;
                    for(s=0;i<subsets.size(); s++){
                        if( !(subsets[s].first.to_ulong() & fullLeft.siteMask) ) break;
                    }
                    //s is now the index of the subset of size subsetSize which has largest SMT and is disjoint from newTop
                    double subsetVal = enablePreprocessing?subsets[s].second:0;


                    lvlCount++;
                    count++;
                    if(enableLowerboundCheck){
                        if(fullLeft.lb+subsetVal>upperBound) continue;

                        fullLeft.lb = lowerBound(fullLeft, tolerance);
                        //if(fullLeft.lb>upperBound) continue;
                        if(fullLeft.lb+subsetVal>upperBound) continue;
                    }

                    for(auto topk = topologies[k-1].begin(); topk!=topologies[k-1].end();++topk){
                        if(topk==topj) break;

                        if(topi->siteMask & topk->siteMask) continue;
                        if(topj->siteMask & topk->siteMask) continue;
                        if(i==j+k && topi->terminalIndex()<topj->terminalIndex() && topi->terminalIndex()<topk->terminalIndex()) continue;

                        Topology full( fullLeft, *topk);
                        if(full.lb>upperBound) break;
                        //myfile<<full.lb<<endl;
                        //full.lb = lowerBound(full, tolerance)-tolerance;
                        full.lb = lowerBound(full, tolerance);
                        count++;
                        lvlCount++;
						if(full.lb<upperBound){
							full.lb = lowerBound(full, tolerance/5);
							if(full.lb<upperBound){
								cout<<"New best full topology ";
								printTop(full);
								cout<<" Len="<<full.lb<<" Ratio="<<full.lb/mstLen<<")"<<endl;
								upperBound = full.lb;
#ifdef DIMACS
								best = Topology(*(new Topology(*topi,*topj)), *topk);
								time_t rawtime;
								time (&rawtime);
								double timeDiff = double(clock()-startTime)/CLOCKS_PER_SEC;
								cout<<"DIMACS Solution "<<timeDiff<<" "<<upperBound<<endl;
#endif
								if(enableLowerboundCheck)
									cleanupTopologies(upperBound, subsets);
							}
                        }
                        //cout<<"Concatenating ";
                        //printTop(*topi); cout<<" ";
                        //printTop(*topj); cout<<" ";
                        //printTop(*topk); cout<<endl;
                        //Topology full()
                    }
                }
            }

            cout<<"> Ran WS on "<<lvlCount<<endl;
        }
    }
    //myfile.close();
    cout<<"Checked "<<count<<" full topologies"<<endl;

    time (&rawtime);
    printf ("Time-stamp: %s", ctime(&rawtime));
	elapsedSecs = (double(clock()-startTime)/CLOCKS_PER_SEC);
    printf ("CPU-time elapsed (in seconds): %.3f\n", elapsedSecs);

#ifdef DIMACS
	printTop(best);cout<<endl;
	double optimal = lowerBound(best,0.01);
	optimal = placement.getLength(0.00001);
	cout<<"DIMACS End"<<endl;
	cout<<"DIMACS SECTION Run"<<endl;
	cout<<"DIMACS Time "<<(double(clock()-startTime)/CLOCKS_PER_SEC)<<endl;
	cout<<"DIMACS Primal "<<optimal<<endl;
	cout<<"DIMACS End"<<endl;
	cout<<"DIMACS SECTION Finalsolution"<<endl;
	cout<<"DIMACS Points "<<(N+N-2)<<endl;
	cout<<"DIMACS Edges "<<(N+N-3)<<endl;
	for(int i=0;i<(N+N-2);i++){
		cout<<"DIMACS ";
		for(int d=0;d<D;d++)
			cout<<"P";
		for(int d=0;d<D;d++)
			cout<<" "<<placement.points[i][d];
		cout<<endl;
	}
	for(int i=0;i<(N-2);i++){
		for(int j=0;j<3;j++){
			if(placement.adj[i][j]<i+N){
				cout<<"DIMACS E "<<placement.adj[i][j]<<" "<<(i+N)<<endl;
			}
		}



	}
	cout<<"DIMACS End"<<endl;
#endif
}

double combinationLowerBound(vector< pair<bitset<MAX_N>, double> > &l1, vector< pair<bitset<MAX_N>, double> > &l2, unsigned long mask)
{
    double minLB = 1000000000.0;
    for(auto i1 = l1.begin(); i1!=l1.end(); ++i1){
        for(auto i2 = l2.begin(); i2!=l2.end(); ++i2){
            if(i1->first==i2->first) break;
            if( (i1->first.to_ulong() | i2->first.to_ulong())==mask  && (i1->second+i2->second<minLB) ){
                minLB = i1->second+i2->second;
            }
        }
    }

    return minLB;
}

void Branch::preprocessLowerbounds(int maxWS, vector< pair<bitset<MAX_N>, double> > &subsets, vector< vector< pair<bitset<MAX_N>, double> > > &preprocessed, double upperBound)
{
    subsets.reserve(100000);

    //vector< vector< pair<bitset<MAX_N>,double> > > preprocessed;
    preprocessed.resize(MAX_N);

    long maxI = 1L<<N;

    cout<<"Preprocessing phase .. "<<endl;
    //cout<<"Computing optimal solutions to subsets of size up to "<<maxWS<<" .. "<<endl;

    //int ssz = maxWS;
    for(int ssz = 3;ssz<=min((uint)maxWS,N-1);ssz++)
	{
        cout<<"Preprocessing subsets of size "<<ssz<<" ";
        if(ssz<=maxWS) cout<<"using Warren-Smith"<<endl;
        else           cout<<"by joining solutions"<<endl;

        for(long i=1; i<maxI; i++){
            pair<bitset<MAX_N>, double> pair(bitset<MAX_N>(i), std::numeric_limits<double>::infinity());
            long subsetSize = pair.first.count();
            if(subsetSize!=ssz) continue;
            //cout<<"> "<<pair.first<<" .. "<<i<<"/"<<maxI<<" - ";

            //if(subsetSize<3) continue;
            if(subsetSize<=maxWS){
                //Transfer included sites to subsetSites
                vector< geomNd::Point > subsetSites(subsetSize, geomNd::Point(placement.D));
                int c=0;
                for(unsigned int j=0;j<N;j++){
                    if(pair.first.test(j)){
                        subsetSites[c++] = sites[j];
                    }
                }

                //Solve using Warren-Smith
                SmithStar ws(subsetSites);
                WSNode* sol = ws.solve(subsetSize, upperBound+0.001);
                pair.second = sol->lowerBound;
                subsets.push_back(pair);
                preprocessed[subsetSize].push_back(pair);


                //PointPlacement placement(subsetSites);
                //sol->updatePlacementAdjacencies(placement);
                //placement.getLength(0.001);
                //bool degenerate=false;
                //for(int s=0;s<subsetSize-2;s++){
                //    geomNd::Point p0(3);
                //    geomNd::Point p1(3);
                //    geomNd::Point p2(3);
                //    geomNd::Point p3(3);
                //    for(int d=0;d<2;d++){
                //        p0[d] = placement.points[s+subsetSize][d];
                //        p1[d] = placement.points[placement.adj[s][0]][d];
                //        p2[d] = placement.points[placement.adj[s][1]][d];
                //        p3[d] = placement.points[placement.adj[s][2]][d];
                //    }
                //    double dist0 = p0.distance(p1);
                //    double dist1 = p0.distance(p2);
                //    double dist2 = p0.distance(p3);
                //    if(fabs(dist0)<0.001){ degenerate = true; break;}
                //    if(fabs(dist1)<0.001){ degenerate = true; break;}
                //    if(fabs(dist2)<0.001){ degenerate = true; break;}
                //}
                //cout<<"Degenerate_test "<<pair.first<<" "<<subsetSize<<" "<<degenerate<<endl;

            }else{
                double lb = upperBound+1.0;
                switch(subsetSize){
                case 20:
                    lb = min(lb, combinationLowerBound(preprocessed[10], preprocessed[10], i));
                    lb = min(lb, combinationLowerBound(preprocessed[9], preprocessed[11], i));
                    lb = min(lb, combinationLowerBound(preprocessed[8], preprocessed[12], i));
                    lb = min(lb, combinationLowerBound(preprocessed[7], preprocessed[13], i));
                    break;
                case 19:
                    lb = min(lb, combinationLowerBound(preprocessed[9], preprocessed[10], i));
                    lb = min(lb, combinationLowerBound(preprocessed[8], preprocessed[11], i));
                    lb = min(lb, combinationLowerBound(preprocessed[7], preprocessed[12], i));
                    break;
                case 18:
                    lb = min(lb, combinationLowerBound(preprocessed[9], preprocessed[9], i));
                    lb = min(lb, combinationLowerBound(preprocessed[8], preprocessed[10], i));
                    lb = min(lb, combinationLowerBound(preprocessed[7], preprocessed[11], i));
                    lb = min(lb, combinationLowerBound(preprocessed[6], preprocessed[12], i));
                    break;
                case 17:
                    lb = min(lb, combinationLowerBound(preprocessed[8], preprocessed[9], i));
                    lb = min(lb, combinationLowerBound(preprocessed[7], preprocessed[10], i));
                    lb = min(lb, combinationLowerBound(preprocessed[6], preprocessed[11], i));
                    break;
                case 16:
                    lb = min(lb, combinationLowerBound(preprocessed[8], preprocessed[8], i));
                    lb = min(lb, combinationLowerBound(preprocessed[7], preprocessed[9], i));
                    lb = min(lb, combinationLowerBound(preprocessed[6], preprocessed[10], i));
                    break;
                case 15:
                    lb = min(lb, combinationLowerBound(preprocessed[7], preprocessed[8], i));
                    lb = min(lb, combinationLowerBound(preprocessed[6], preprocessed[9], i));
                    lb = min(lb, combinationLowerBound(preprocessed[5], preprocessed[10], i));
                    break;
                case 14:
                    lb = min(lb, combinationLowerBound(preprocessed[7], preprocessed[7], i));
                    lb = min(lb, combinationLowerBound(preprocessed[6], preprocessed[8], i));
                    lb = min(lb, combinationLowerBound(preprocessed[5], preprocessed[9], i));
                    break;
                case 13:
                    lb = min(lb, combinationLowerBound(preprocessed[5], preprocessed[8], i));
                    lb = min(lb, combinationLowerBound(preprocessed[6], preprocessed[7], i));
                    break;
                case 12:
                    lb = min(lb, combinationLowerBound(preprocessed[6], preprocessed[6], i));
                    lb = min(lb, combinationLowerBound(preprocessed[5], preprocessed[7], i));
                    lb = min(lb, combinationLowerBound(preprocessed[4], preprocessed[8], i));
                    break;
                case 11:
                    lb = min(lb, combinationLowerBound(preprocessed[5], preprocessed[6], i));
                    lb = min(lb, combinationLowerBound(preprocessed[4], preprocessed[7], i));
                    break;
                case 10:
                    lb = min(lb, combinationLowerBound(preprocessed[5], preprocessed[5], i));
                    lb = min(lb, combinationLowerBound(preprocessed[4], preprocessed[6], i));
                    break;
                case 9:
                    lb = min(lb, combinationLowerBound(preprocessed[4], preprocessed[5], i));
                    lb = min(lb, combinationLowerBound(preprocessed[3], preprocessed[6], i));
                    break;
                case 8:
                    lb = min(lb, combinationLowerBound(preprocessed[4], preprocessed[4], i));
                    lb = min(lb, combinationLowerBound(preprocessed[3], preprocessed[5], i));
                    break;
                case 7:
                    lb = min(lb, combinationLowerBound(preprocessed[4], preprocessed[3], i));
                    break;
                case 6:
                    lb = min(lb, combinationLowerBound(preprocessed[3], preprocessed[3], i));
                    lb = min(lb, combinationLowerBound(preprocessed[2], preprocessed[4], i));
                    break;
                }

                pair.second = lb;
                preprocessed[subsetSize].push_back(pair);
                subsets.push_back(pair);
                //cout<<pair.second<<endl;
            }
        }
    }
    //throw "Branch::preprocessLowerbounds .. stopping prematurely";

    //Sort
    sort(subsets.begin(), subsets.end(), sort_by_second());
}


void Branch::cleanupTopologies(double upperBound, vector< pair<bitset<MAX_N>, double> > &subsets)
{
    std::unordered_set<Topology*> delSet;
    for( unsigned int k=1; k<topologies.size(); k++){
        if(topologies[k].size()==0) break;

        for( auto topit = topologies[k].begin(); topit!=topologies[k].end(); ++topit){

            auto found = delSet.find(topit->left);
            if(found!=delSet.end()) { delSet.insert( &(*topit) ); continue; }
            found = delSet.find(topit->right);
            if(found!=delSet.end()) { delSet.insert( &(*topit) ); continue; }

            size_t maxSubset;
            for(maxSubset=0;maxSubset<subsets.size(); maxSubset++){
                if( !(subsets[maxSubset].first.to_ulong() & topit->siteMask) ) break;
            }
            //maxSubset is now the index of the subset which has largest SMT and is disjoint from topit
            double subsetVal = enablePreprocessing?subsets[maxSubset].second:0;

            if( topit->lb+subsetVal > upperBound ) {
                delSet.insert(&(*topit));
            }
        }
    }

    cout<<"Pruning: ";
    //Erase existing k-level tops based on lb
    for( unsigned int k=2; k<topologies.size(); k++){
        if(topologies[k].size()==0) break;

        int eraseCount = 0;
        for( auto topit = topologies[k].begin(); topit!=topologies[k].end(); ){

            //size_t maxSubset;
            //for(maxSubset=0;maxSubset<subsets.size(); maxSubset++){
            //    if( !(subsets[maxSubset].first.to_ulong() & topit->siteMask) ) break;
            //}
            ////maxSubset is now the index of the subset which has largest SMT and is disjoint from topit

            //if( topit->lb+subsets[maxSubset].second > upperBound ) {
            auto found = delSet.find(&(*topit));
            if(found!=delSet.end()){
                topit = topologies[k].erase(topit);
                eraseCount++;
            }else{
                ++topit;
            }
        }
        cout<<"("<<(k+1)<<","<<eraseCount<<") ";
    }
    cout<<endl;

}

void Branch::printTopologies()
{
    for(size_t lvl=0;lvl<N;lvl++){
        cout<<"Lvl "<<lvl+1<<" :"<<topologies[lvl].size()<<endl;
    }
}

void Branch::printTopologies(uint lvl)
{
    cout<<"Lvl "<<lvl<<" topologies:"<<endl;
    //for(size_t t=0;t<topologies[lvl].size();t++){
    //    printTop(topologies[lvl][t]);
    for(auto it=topologies[lvl].begin(); it!=topologies[lvl].end(); ++it){
        printTop(*it);
        cout<<endl;
    }
}
void Branch::printTop(Topology &top)
{
    //if(top.lvl1==-1)
    if(top.left==0)
    {
        //cout<<top.idx1;
        cout<<top.terminalIndex();
        return;
    }
    cout<<"(";
    //printTop(topologies[top.lvl1][top.idx1]);
    printTop(*(top.left));
    cout<<",";
    //printTop(topologies[top.lvl2][top.idx2]);
    printTop(*(top.right));
    cout<<")";
}

//Goes through all terminals to check if any of them are contained in the interior of the
//lens between p1 and p2
//@param p1 Index of terminal 1
//@param p2 Index of terminal 2
//@return true iff there is no terminal in lens(p1, p2)
bool Branch::lensCheck(uint p1, uint p2)
{
    for(size_t i=0;i<N;i++){
        if(i==(uint)p1 || i==(uint)p2) continue;
        double angle = geomNd::Point::angle(sites[p1], sites[i], sites[p2]);
        if( angle > 2*M_PI/3.0 )
            return false;
    }
    return true;
}

//Checks the two edges of the cherry to see if they will violate the bottleneck criterion
//when concatenated with a terminal
//@return true iff there exists a configuration of the cherry concatenated with the terminal
//such that the longest edge is no longer than the bottleneck distance.
bool Branch::lensCheck(Topology &cherry, Topology &term)
{
    int c0Site = cherry.left->terminalIndex();  //Index of cherry-terminal 0
    int c1Site = cherry.right->terminalIndex(); //Index of cherry-terminal 1
    int c2Site = term.terminalIndex();          //Index of terminal

    double a012 = geomNd::Point::angle(sites[c0Site], sites[c1Site], sites[c2Site] );
    double a102 = geomNd::Point::angle(sites[c2Site], sites[c0Site], sites[c1Site] );

    //Longest edge goes from c0-c1 which would have been pruned earlier
    if( a012>M_PI/6 || a102>M_PI/6 ) return true;

    double a = sites[c0Site].distance(sites[c1Site]); // |c0-c1|
    double b = sites[c1Site].distance(sites[c2Site]); // |c1-c2|
    double c = sites[c2Site].distance(sites[c0Site]); // |c2-c0|
    double h = geom2d::Triangle::height(b,c,a); // Distance from c2 to triangle base line

    //Create normalized planar coordinates of c0, c1, and c2
    geom2d::Point p0(0,0); //Corresponds to c0
    geom2d::Point p1(a,0); //Corresponds to c1
    geom2d::Point p2( (c*c-b*b+a*a)/(c+c) , h); //Corresponds to c2

    //Find equilateral point and circle of (c0,c1)
    geom2d::Circle eqcir01 = geom2d::Point::equilateralCircle(p0,p1,-1); //TODO: Optimize using p0=(0,0) and p1[1]=0
    geom2d::Point eq01 = geom2d::Point::equilateralPoint(p0,p1,-1); //TODO: Optimize using p0=(0,0) and p1[1]=0


    if(a102<M_PI/6){ //Edge from c1 is longer than anticipated by normal bottleneck pruning
        geom2d::Circle eqcirE2 = geom2d::Point::equilateralCircle(eq01,p2,-1);

        geom2d::Point planeSteiner = eqcir01.intersection(eqcirE2, 1);
        double min_p0_st = p0.distance(planeSteiner);
        if( min_p0_st>longestMSTDists[c0Site][c1Site]) return false;
        if( min_p0_st>longestMSTDists[c0Site][c2Site]) return false;

    }else{ //Edge from c0 is longer than anticipated by normal bottleneck pruning
        geom2d::Circle eqcirE2 = geom2d::Point::equilateralCircle(eq01,p2, 1);

        geom2d::Point planeSteiner = eqcir01.intersection(eqcirE2, 1);
        double min_p1_st = p1.distance(planeSteiner);

        if( min_p1_st>longestMSTDists[c0Site][c1Site]) return false;
        if( min_p1_st>longestMSTDists[c1Site][c2Site]) return false;

    }

    return true;
}

/**
 * @brief Collect all terminals and terminal-depths contained in top into the two vectors.
 * @param top Topology
 * @param depth Accumulator used for recursive calls. Should be 0 when called outside of function.
 * @param sites Vector for storing terminals
 * @param depths Vector for storing depths
 */
void Branch::collectSites(Topology &top, uint depth, vector<uint> &sites, vector<uint> &depths)
{
    if(top.left==0)
    {
        sites.push_back(top.terminalIndex());
        depths.push_back(depth);
        return;
    }

    collectSites(*(top.left) , depth+1, sites, depths);
    collectSites(*(top.right), depth+1, sites, depths);
}

/**
 * @brief Branch::overlapCheck performs O(n^2) check if two vectors contain the same terminal
 * @param sites1 Vector 1
 * @param sites2 Vector 2
 * @return true iff the same element does not appear in both vectors
 */
bool Branch::overlapCheck(vector<uint> &sites1, vector<uint> &sites2)
{
    for(size_t i=0;i<sites1.size();i++){
        for(size_t j=0;j<sites2.size();j++){
            if(sites1[i]==sites2[j]) return false;
        }
    }

    return true;
}

bool Branch::bottleneckCheck(vector<uint> &sites1, vector<uint> &depths1, vector<uint> &sites2, vector<uint> &depths2)
{
    for(size_t i=0; i<sites1.size(); i++){
        uint sitei = sites1[i];
        uint depthi = depths1[i];
        for(size_t j=0; j<sites2.size(); j++){
            uint sitej = sites2[j];
            uint depthj = depths2[j];

            if(bottleneckDists[sitei][sitej]>depthi+depthj+2) return false;
        }
    }
    return true;
}

uint Branch::computeBottleneckDists()
{
    cout<<"Bottleneck matrix: "<<endl;
    uint maxSteps = 0;
    MST mst(sites);
    for(size_t i=0;i<N;i++){
        for(size_t j=0;j<i;j++) cout<<"    ";

        for(size_t j=i+1;j<N;j++){
            double dist = sites[i].distance(sites[j]);
            double longestMSTedge = mst.longestEdge(i,j);
            longestMSTDists[i][j] = longestMSTDists[j][i] = longestMSTedge;
            bottleneckDists[i][j] = bottleneckDists[j][i] = (int)( (dist*2)/(sqrt(3)*longestMSTedge) );
            //bottleneckDists[i][j] = bottleneckDists[j][i] = (int)( dist/(longestMSTedge+0.000001) );
            printf("%4d", bottleneckDists[i][j]);
            if(bottleneckDists[i][j]>maxSteps ) maxSteps = bottleneckDists[i][j];
        }
        cout<<endl;
    }
    cout<<endl;

    return maxSteps;
}

int Branch::setAdj(Topology &top, int p, int &c)
{
    //if(top.lvl1<0)
    if(top.left==0)
    {
        return top.terminalIndex();
        //return top.idx1;
    }else{
        int steinerIdx = c;
        int pointIdx = c+N;
        c++;
        placement.adj[steinerIdx][0] = p;
        placement.adj[steinerIdx][1] = setAdj( *(top.left) , pointIdx, c );
        placement.adj[steinerIdx][2] = setAdj( *(top.right), pointIdx, c );
        //placement.adj[steinerIdx][1] = setAdj( topologies[top.lvl1][top.idx1], pointIdx, c );
        //placement.adj[steinerIdx][2] = setAdj( topologies[top.lvl2][top.idx2], pointIdx, c );
        return pointIdx;
    }
}

double Branch::lowerBound(Topology &top, double tolerance)
{
    //Set the adj of placement
    int c=0;
    int sp1 = setAdj( *(top.left) ,  -1, c);
    int sp2 = setAdj( *(top.right), sp1, c);
    //int sp1 = setAdj(topologies[top.lvl1][top.idx1], -1, c);
    //int sp2 = setAdj(topologies[top.lvl2][top.idx2], sp1, c);
    if(sp1>=N) placement.adj[0][0] = sp2;
    placement.steinerPoints = c;

    for(int i=0;i<c;i++){
        if(placement.adj[i][0]>placement.adj[i][1]){ int tmp = placement.adj[i][0]; placement.adj[i][0] = placement.adj[i][1]; placement.adj[i][1] = tmp; }
        if(placement.adj[i][1]>placement.adj[i][2]){ int tmp = placement.adj[i][1]; placement.adj[i][1] = placement.adj[i][2]; placement.adj[i][2] = tmp; }
        if(placement.adj[i][0]>placement.adj[i][1]){ int tmp = placement.adj[i][0]; placement.adj[i][0] = placement.adj[i][1]; placement.adj[i][1] = tmp; }
    }

    lbLevels[c+2]++;

    //Compute length
    //return placement.getLength(tolerance);
    return placement.getLowerbound(tolerance);
}

void Branch::printLBEvals()
{

    int totalEval = 0;
    float avg = 0;
    for(int i=3;i<100;i++){
        if(lbLevels[i]==0) continue;
        cout<<"Evaluated_Level: "<<i<<" "<<lbLevels[i]<<"\n";
        totalEval+=lbLevels[i];
        avg+=lbLevels[i]*i;
    }
    avg/=totalEval;

    cout<<"Evaluated_Nodes: "<<totalEval<<"\n";
    cout<<"Avg_Node_Size: "<<avg<<"\n";

}

