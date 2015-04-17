#include "ComponentExperiment.h"

#include "geomNd/PointND.h"
#include "SmithStar/SmithStar.h"
#include "MST.h"
#include "PointPlacement.h"
#include "Utils.h"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <math.h>



using namespace geomNd;

ComponentExperiment::ComponentExperiment()
{
}

void ComponentExperiment::runWOPruning()
{
    std::default_random_engine generator(2);
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    for(int d=2;d<=5;d++){
        for(int n=20;n<=100;n+=20){
            //Generate n d-dimensional points
            std::vector<Point> points;
            for(int i=0;i<n;i++){
                Point p(d);
                for(int j=0;j<d;j++)
                    p[j] = distribution(generator);
                points.push_back(p);
            }


            for(int sz=3;sz<=8;sz+=1){
                std::cout<<"d = "<<d<<" ; n = "<<n<<" ; sz = "<<sz;

                int degs = 0;
                int bots = 0;
                int luns = 0;
                int alls = 0;
                int iterations = 1000;
                for(int it=0;it<iterations;it++){

                    //Randomize points so first sz represents a random subset
                    std::shuffle(points.begin(), points.end(), generator );

                    //std::cout<<"d = "<<d<<" ; n = "<<n<<" ; sz = "<<sz<<" ; iteration = "<<it;
                    //std::cout<<std::endl;
                    //for(int i=0;i<n;i++)
                    //    std::cout<<points[i]<<std::endl;

                    //Find SMT for first sz elements in points
                    std::vector<Point> subsetPoints;
                    for(int i=0;i<sz;i++)
                        subsetPoints.push_back(points[i]);

                    SmithStar bnb(subsetPoints);
                    MST mst(subsetPoints);
                    WSNode* n = bnb.solve(sz, mst.getLength()+0.001);
                    PointPlacement placement(subsetPoints);
                    n->updatePlacementAdjacencies(placement);
                    placement.getLength(0.0001);
                    //std::cout<<placement.getLength(0.0001)<<std::endl;
                    bool pruned_by_degeneracy = isDeg(placement);
                    //std::cout<<" ; degenerate = "<<deg;
                    if(!pruned_by_degeneracy) degs++;

                    std::vector< std::vector<double> > mst_bottlenecks = mstBottleneckDistances(points);
                    bool pruned_by_bottleneck = prunedByBottleneck(placement, mst_bottlenecks);
                    if(!pruned_by_bottleneck) bots++;


                    bool pruned_by_lunes = prunedByLune(placement, points);
                    if(!pruned_by_lunes) luns++;

                    if(!pruned_by_degeneracy && !pruned_by_bottleneck && !pruned_by_lunes) alls++;

                    //std::cout<<std::endl;

                }
                std::cout<<" ; degs = ";
				std::cout.width(6);
				std::cout<<degs<<" ; bots = ";
				std::cout.width(6);
				std::cout<<bots<<" ; luns = ";
				std::cout.width(6);
				std::cout<<luns<<" ; alls = ";
				std::cout.width(6);
				std::cout<<alls<<" ; tot = ";
				std::cout.width(6);
				std::cout<<iterations;
                std::cout<<std::endl;
            }


        }
    }
}

void ComponentExperiment::runWPruning()
{

}

bool ComponentExperiment::prunedByBottleneck(PointPlacement& placement, std::vector< std::vector<double> >& bottleneck_dists)
{
    std::vector< std::vector<double> > stp_bottlenecks = steinerTreeBottleneckDistances(placement);

    for(int i1=0;i1<placement.N;i1++){
        for(int i2=i1+1;i2<placement.N;i2++){
            if(stp_bottlenecks[i1][i2]+0.001>bottleneck_dists[i1][i2]) return true;
        }
    }
    return false;
}


void ComponentExperiment::collectTerminals(PointPlacement& placement, int root, int rootParent, std::vector<int>& ret)
{
    if(root<placement.N) {
        ret.push_back(root);
    }else{
        if(placement.adj[root-placement.N][0]!=rootParent) collectTerminals(placement, placement.adj[root-placement.N][0], root, ret);
        if(placement.adj[root-placement.N][1]!=rootParent) collectTerminals(placement, placement.adj[root-placement.N][1], root, ret);
        if(placement.adj[root-placement.N][2]!=rootParent) collectTerminals(placement, placement.adj[root-placement.N][2], root, ret);
    }
}
std::vector< std::vector<double> > ComponentExperiment::mstBottleneckDistances(std::vector<Point>& points)
{
    std::vector< std::vector<double> > ret(points.size(), std::vector<double>(points.size(), 0.0));
    MST mst(points);

    for(int i1=0;i1<points.size();i1++){
        for(int i2=i1+1;i2<points.size();i2++){
            ret[i1][i2] = ret[i2][i1] = std::max(ret[i1][i2], mst.longestEdge(i1,i2));
        }
    }

    return ret;
}

std::vector< std::vector<double> > ComponentExperiment::steinerTreeBottleneckDistances(PointPlacement& placement)
{
    std::vector< std::vector<double> > ret(placement.N, std::vector<double>(placement.N, 0.0));

    std::vector<int> terminals1;
    std::vector<int> terminals2;
    for(int s=0;s<placement.steinerPoints;s++){
        Point p0(placement.D); for(int i=0;i<placement.D;i++) p0[i] = placement.points[placement.N+s][i];
        Point p1(placement.D); for(int i=0;i<placement.D;i++) p1[i] = placement.points[placement.adj[s][0]][i];
        Point p2(placement.D); for(int i=0;i<placement.D;i++) p2[i] = placement.points[placement.adj[s][1]][i];
        Point p3(placement.D); for(int i=0;i<placement.D;i++) p3[i] = placement.points[placement.adj[s][2]][i];

        double d1 = p0.distance(p1);
        double d2 = p0.distance(p2);
        double d3 = p0.distance(p3);

        terminals1.clear(); terminals2.clear();
        collectTerminals(placement, s+placement.N, placement.adj[s][0], terminals1);
        collectTerminals(placement, placement.adj[s][0], s+placement.N, terminals2);
        for(int i1=0;i1<terminals1.size();i1++){
            for(int i2=0;i2<terminals2.size();i2++){
                ret[terminals1[i1]][terminals2[i2]] = std::max(ret[terminals1[i1]][terminals2[i2]], d1);
                ret[terminals2[i2]][terminals1[i1]] = ret[terminals1[i1]][terminals2[i2]];
            }
        }

        terminals1.clear(); terminals2.clear();
        collectTerminals(placement, s+placement.N, placement.adj[s][1], terminals1);
        collectTerminals(placement, placement.adj[s][1], s+placement.N, terminals2);
        for(int i1=0;i1<terminals1.size();i1++){
            for(int i2=0;i2<terminals2.size();i2++){
                ret[terminals1[i1]][terminals2[i2]] = std::max(ret[terminals1[i1]][terminals2[i2]], d2);
                ret[terminals2[i2]][terminals1[i1]] = ret[terminals1[i1]][terminals2[i2]];
            }
        }

        terminals1.clear(); terminals2.clear();
        collectTerminals(placement, s+placement.N, placement.adj[s][2], terminals1);
        collectTerminals(placement, placement.adj[s][2], s+placement.N, terminals2);
        for(int i1=0;i1<terminals1.size();i1++){
            for(int i2=0;i2<terminals2.size();i2++){
                ret[terminals1[i1]][terminals2[i2]] = std::max(ret[terminals1[i1]][terminals2[i2]], d3);
                ret[terminals2[i2]][terminals1[i1]] = ret[terminals1[i1]][terminals2[i2]];
            }
        }
    }

    return ret;
}



bool ComponentExperiment::prunedByLune(PointPlacement& placement, std::vector< Point >& points)
{
    double angle_120 = M_PI*2.0/3.0;
    for(int s=0;s<placement.steinerPoints;s++){
        Point p0(placement.D); for(int i=0;i<placement.D;i++) p0[i] = placement.points[placement.N+s][i];
        Point p1(placement.D); for(int i=0;i<placement.D;i++) p1[i] = placement.points[placement.adj[s][0]][i];
        Point p2(placement.D); for(int i=0;i<placement.D;i++) p2[i] = placement.points[placement.adj[s][1]][i];
        Point p3(placement.D); for(int i=0;i<placement.D;i++) p3[i] = placement.points[placement.adj[s][2]][i];

        for(int i=placement.N;i<points.size();i++){
            Point& p = points[i];
            double a1 = Point::angle(p0,p,p1);
            double a2 = Point::angle(p0,p,p2);
            double a3 = Point::angle(p0,p,p3);

            //std::cout<<endl;
            //std::cerr<<p0<<" ; "<<p1<<" ; "<<p2<<" ; "<<p3<<" ; "<<p;
            //std::cerr<<" ; a1 = "<<(a1*180/M_PI);
            //std::cerr<<" ; a2 = "<<(a2*180/M_PI);
            //std::cerr<<" ; a3 = "<<(a3*180/M_PI);
            //std::cerr<<endl;
            if(a1>angle_120) {
                //std::cerr<<"Pruning by lune: "<<p0<<" ; "<<p<<" ; "<<p1<<" ; angle = "<<(a1*180/M_PI)<<std::endl;
                return true;
            }
            if(a2>angle_120) {
                //std::cerr<<"Pruning by lune: "<<p0<<" ; "<<p<<" ; "<<p2<<" ; angle = "<<(a2*180/M_PI)<<std::endl;
                return true;
            }
            if(a3>angle_120) {
                //std::cerr<<"Pruning by lune: "<<p0<<" ; "<<p<<" ; "<<p3<<" ; angle = "<<(a3*180/M_PI)<<std::endl;
                return true;
            }
            //exit(-1);
        }
    }

    return false;
}


bool ComponentExperiment::isDeg(PointPlacement& placement)
{
    for(int s=0;s<placement.steinerPoints;s++){
        Point p0(placement.D); for(int i=0;i<placement.D;i++) p0[i] = placement.points[placement.N+s][i];
        Point p1(placement.D); for(int i=0;i<placement.D;i++) p1[i] = placement.points[placement.adj[s][0]][i];
        Point p2(placement.D); for(int i=0;i<placement.D;i++) p2[i] = placement.points[placement.adj[s][1]][i];
        Point p3(placement.D); for(int i=0;i<placement.D;i++) p3[i] = placement.points[placement.adj[s][2]][i];

        if(p0.distance(p1)<0.001) return true;
        if(p0.distance(p2)<0.001) return true;
        if(p0.distance(p3)<0.001) return true;
    }
    return false;
}
