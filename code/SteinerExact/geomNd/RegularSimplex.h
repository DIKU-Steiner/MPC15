#ifndef REGULARSIMPLEX_H
#define REGULARSIMPLEX_H

#include <vector>
#include "geomNd/Simplex.h"

using namespace std;

class RegularSimplex: public Simplex
{
public:
    /** Computes a regular dim-dimensional simplex with one corner (a point) */
    RegularSimplex(int dim);

    // /** If s has fewer than s.dimensions()+1 corners this method will calculate a new regular simplex
    //  * with the all the corners of s and one more. */
    // RegularSimplex lift(RegularSimplex &s);
};

#endif // REGULARSIMPLEX_H
