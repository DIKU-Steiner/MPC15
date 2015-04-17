
#ifndef PROBLEMINSTANCE_H
#define PROBLEMINSTANCE_H

#include <vector>
#include <string>

#include "geomNd/PointND.h"


using namespace std;

class ProblemInstance{
	public:
		vector< geomNd::Point > sites;
		string name;
		string program;
		string version;
		string problem;

		double optimalValue;
		vector< geomNd::Point > best;
		double time;

		ProblemInstance();

		void writeToFile(const char* fName);
		void readFromFile(const char* fName);
};

#endif // PROBLEMINSTANCE_H
