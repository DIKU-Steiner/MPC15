#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <vector>

class Simplex
{
public:
    Simplex(std::vector< std::vector<double> > corners);

protected:
    unsigned int dim;
    std::vector< std::vector<double> > corners;
};

#endif // SIMPLEX_H
