#include "IO.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "geomNd/PointND.h"

using namespace std;

IO::IO()
{
}

/**
 * @brief IO::readSTPFromFile reads an STP file from a path-name.
 * Assumes that only one set of sites are contained in the file.
 * @param fName Path to file
 * @param sites Reference to vector that will contain the coordinates from the file
 */
void IO::readSTPFromFile(const char *fName, vector< geomNd::Point > &sites)
{
	cout<<"Terminals from "<<fName<<endl;
#ifdef DIMACS
	cout<<"DIMACS SECTION Comment"<<endl;
#endif

    ifstream infile(fName);
    string line;
    bool readingCoords = false;
    int lineNum = 0;
	int siteCount = 0;
    while (getline(infile, line)){
        lineNum++;
#ifdef DIMACS
        if(line.find("Name")!=string::npos || line.find("Problem")!=string::npos){
			cout<<"DIMACS "<<line<<endl;
		}
#endif
        if(line.find("SECTION Coordinates")!=string::npos){
            readingCoords = true;
            continue;
        }
        if(line.find("END")!=string::npos){
            readingCoords = false;
            continue;
        }
        if(readingCoords){
            istringstream iss(line);
            int idx;
            string ds;
            vector<double> coords;
            if(!(iss>>ds>>idx)){ cerr<<"Error parsing line "<<lineNum<<": "<<line<<endl; throw "IOError";}

            for(int i=0;i<ds.length();i++){
                double c;
                if( !(iss>>c) ){ cerr<<"Error parsing coordinates line "<<lineNum<<": "<<line<<endl; throw "IOError"; }
                coords.push_back(c);
            }
            sites.push_back(geomNd::Point(coords));
			cout<<"> terminal["<<(sites.size()-1)<<"] = "<<sites[sites.size()-1]<<endl;
        }


    }
    infile.close();

#ifdef DIMACS
	cout<<"DIMACS Program \"Branch\""<<endl;
	cout<<"DIMACS Version \"0.01\""<<endl;
	cout<<"DIMACS End"<<endl;
#endif
}

/**
 * @brief IO::readFromRandSphere generates N points uniformly distributed within a D-dimensional
 * hypersphere.
 * @param D Dimension
 * @param N Number of sites
 * @param sites Reference to vector that will contain the point-coordinates
 */
void IO::readFromRandSphere(const int D, const int N, vector< geomNd::Point > &sites)
{
    cout<<"Random terminals in hyper-cube:"<<endl;
    sites.resize(N, geomNd::Point(D));

    for(unsigned int i=0;i<sites.size();i++){
        //double distSq = 2;
        //while(distSq>1)
        {
            //distSq = 0;
            for(int d=0;d<D;d++){
                sites[i][d] = (double)rand()/RAND_MAX;
                //distSq+=sites[i][d]*sites[i][d];
            }
        }

        cout<<"> terminal["<<i<<"] = "<<sites[i]<<endl;
        //printf("> terminal[%d] = (", i);
        //for(int d=0;d<D-1;d++) printf("%.8f, ",sites[i][d]);
        //printf("%.8f)\n",sites[i][D-1]);
    }
}
