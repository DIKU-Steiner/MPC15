#include "RegularSimplex.h"

#include <math.h>

#include "Utils.h"

RegularSimplex::RegularSimplex(int dim): Simplex( std::vector< std::vector<double> >(1, std::vector<double>(dim, 0)) )
{}

//RegularSimplex RegularSimplex::lift(RegularSimplex &s)
//{
//    //Find centroid of existing corners
//    vector< double > p(dim, 0);
//    for(unsigned int c=0; c<s.corners.size(); c++)
//    {
//        p = Utils::add(p, s.corners[c]);
//    }
//    p = Utils::div(p, s.corners.size());
//
//    //Lift into next dimension
//    double desired = 1;
//    if(s.corners.size()>2) desired = Utils::dist(s.corners[0], s.corners[1]);
//    double actual = Utils::dist(p, s.corners[0]);
//    double liftHeight = sqrt(desired*desired - actual*actual);
//    //TODO: Finish
//}
