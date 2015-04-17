#include "TestWSConvergence.h"


#include <math.h>
#include <string.h> //memset
#include <iomanip>

#include "Utils.h"
#include "geomNd/PointND.h"

using namespace std;

TestWSConvergence::TestWSConvergence(vector< geomNd::Point > sites):
    D(sites[0].dimensions()),
    N(sites.size())
{
    for(int i=0;i<98;i++)  memset(points[i],0,10*sizeof(double));
    for(int i=0;i<48;i++)  memset(adj[i],0,3*sizeof(int));
    for(int i=0;i<48;i++)  memset(EL[i],0,3*sizeof(double));
    for(int i=0;i<50;i++)  memset(B[i],0,3*sizeof(double));
    for(int i=0;i<50;i++)  memset(C[i],0,10*sizeof(double));
    memset(eqnstack,0,50*sizeof(int));
    memset(leafQ,0,50*sizeof(int));
    memset(val,0,50*sizeof(int));


    for(int i=0;i<N; i++){
        for(int d=0;d<D; d++){
            points[i][d] = sites[i][d];
        }
    }

    steinerPoints = 0;
}

double TestWSConvergence::getLowerbound(const double tol)
{
    return getLength(tol)-error();
}
double TestWSConvergence::getLength(const double tol)
{
    double q,r;
    do{
        q = length();
        r = error();
        optimize(tol*r/N);
    }while(r>tol*q);

    return q;
}

double TestWSConvergence::length()
{
    int i2,j,m;
    int n0,n1,n2;
    double leng = 0.0, t, r;
    for(int i=0; i<steinerPoints; i++){
        i2 = i + N;
        n0 = adj[i][0]; n1 = adj[i][1]; n2 = adj[i][2];
        if(n0<i2){
            t=0.0;for(m=0; m<D; m++){ r=points[n0][m]-points[i2][m]; t +=r*r; } t=sqrt(t);
            //t = Utils::dist(points[n0], points[i2]);
            //t = points[n0].distance(points[i2]);
            leng += t; EL[i][0] = t; n0 -= N;
            if(n0>=0) for(j=0; j<3; j++) if(adj[n0][j]==i2){ EL[n0][j] = t; break; }
        }
        if(n1<i2){
            t=0.0;for(m=0; m<D; m++){ r=points[n1][m]-points[i2][m]; t +=r*r; } t=sqrt(t);
            //t = Utils::dist(points[n1], points[i2]);
            //t = points[n1].distance(points[i2]);
            leng += t; EL[i][1] = t; n1 -= N;
            if(n1>=0) for(j=0; j<3; j++) if(adj[n1][j]==i2){ EL[n1][j] = t; break; }
        }
        if(n2<i2){
            t=0.0;for(m=0; m<D; m++){ r=points[n2][m]-points[i2][m]; t +=r*r; } t=sqrt(t);
            //t = Utils::dist(points[n2], points[i2]);
            //t = points[n2].distance(points[i2]);
            leng += t; EL[i][2] = t; n2 -= N;
            if(n2>=0) for(j=0; j<3; j++) if(adj[n2][j]==i2){ EL[n2][j] = t; break; }
        }
    }/* Have now figured out distance EL[i][0..2] from Steiner pt. i to neighbors. */
    return leng;
}

double TestWSConvergence::lengthDebug()
{
    int i2,j,m;
    int n0,n1,n2;
    double leng = 0.0, t, r;
    for(int i=0; i<steinerPoints; i++){
        i2 = i + N;
        n0 = adj[i][0]; n1 = adj[i][1]; n2 = adj[i][2];
        if(n0<i2){
            t=0.0;for(m=0; m<D; m++){ r=points[n0][m]-points[i2][m]; t +=r*r; } t=sqrt(t);
            cout<<"Point["<<n0<<"] to Point["<<i2<<"] (EL["<<i<<"][0]): ";
            cout<<t<<endl;
            //t = Utils::dist(points[n0], points[i2]);
            //t = points[n0].distance(points[i2]);
            leng += t; EL[i][0] = t; n0 -= N;
            if(n0>=0) for(j=0; j<3; j++) if(adj[n0][j]==i2){ EL[n0][j] = t; break; }
        }
        if(n1<i2){
            t=0.0;for(m=0; m<D; m++){ r=points[n1][m]-points[i2][m]; t +=r*r; } t=sqrt(t);
            //t = Utils::dist(points[n1], points[i2]);
            //t = points[n1].distance(points[i2]);
            cout<<"Point["<<n1<<"] to Point["<<i2<<"] (EL["<<i<<"][1]): ";
            cout<<t<<endl;
            leng += t; EL[i][1] = t; n1 -= N;
            if(n1>=0) for(j=0; j<3; j++) if(adj[n1][j]==i2){ EL[n1][j] = t; break; }
        }
        if(n2<i2){
            t=0.0;for(m=0; m<D; m++){ r=points[n2][m]-points[i2][m]; t +=r*r; } t=sqrt(t);
            //t = Utils::dist(points[n2], points[i2]);
            //t = points[n2].distance(points[i2]);
            cout<<"Point["<<n2<<"] to Point["<<i2<<"] (EL["<<i<<"][2]): ";
            cout<<t<<endl;
            leng += t; EL[i][2] = t; n2 -= N;
            if(n2>=0) for(j=0; j<3; j++) if(adj[n2][j]==i2){ EL[n2][j] = t; break; }
        }
    }/* Have now figured out distance EL[i][0..2] from Steiner pt. i to neighbors. */
    return leng;
}

double TestWSConvergence::error()
{
    int i,m,i2,n0,n1,n2;
    double r,s,t,d01,d12,d02;
    double efig=0.0;
    for(i=0; i<steinerPoints; i++){
        i2 = i+N;
        n0 = adj[i][0]; n1 = adj[i][1]; n2 = adj[i][2];
        d12 = d01 = d02 = 0.0;
        for(m=0; m<D; m++){
            t = points[i2][m];
            r = points[n0][m]-t; s = points[n1][m]-t; t = points[n2][m]-t;
            d12 += s*t; d01 += r*s; d02 += r*t;
        }
        // only angles < 120 cause error
        t = d12 + d12 + EL[i][1]*EL[i][2]; if(t>0.0) efig += t;
        t = d01 + d01 + EL[i][0]*EL[i][1]; if(t>0.0) efig += t;
        t = d02 + d02 + EL[i][0]*EL[i][2]; if(t>0.0) efig += t;
    }
    return sqrt(efig);

}

void TestWSConvergence::optimize(const double tol)
{
    int m,j,i2, i;
    int n0,n1,n2,lqp,eqp;
    double q0,q1,q2,t;
    lqp = eqp = 0;

    // First: compute B array, C array, and valences. Set up leafQ.
    for(i=steinerPoints-1; i>=0; i--){
        n0 = adj[i][0]; n1 = adj[i][1]; n2 = adj[i][2];
        q0 = 1.0/(EL[i][0]+tol);
        q1 = 1.0/(EL[i][1]+tol);
        q2 = 1.0/(EL[i][2]+tol);
        //Have now figured out reciprocal distances q0,q1,q2 from
        //Steiner pt. i to neighbors n0,n1,n2 **/
        t = q0+q1+q2; q0/=t; q1/=t; q2/=t;
        val[i] = 0; B[i][0] = B[i][1] = B[i][2] = 0.0;
        for(m=0; m<D; m++){ C[i][m] = 0.0; }
        //if(n0>N){ val[i]++; B[i][0] = q0; } else for(m=0; m<D; m++) C[i][m] += XX[n0][m]*q0;
        if(n0>=N){ val[i]++; B[i][0] = q0; } else for(m=0; m<D; m++) C[i][m] += points[n0][m]*q0;
        if(n1>=N){ val[i]++; B[i][1] = q1; } else for(m=0; m<D; m++) C[i][m] += points[n1][m]*q1;
        if(n2>=N){ val[i]++; B[i][2] = q2; } else for(m=0; m<D; m++) C[i][m] += points[n2][m]*q2;
        //Now: Steiner point i has Steiner valence val[i];
        //coords obey eqns XX[i+N][] = sum(j)of B[i][j]*XX[nj][] + C[i][] */
        if(val[i] <= 1){ leafQ[lqp] = i; lqp++; }/* put leafs on leafQ */

    }

    //Have set up equations - now-to solve them. */
    //Second: eliminate leaves */
    while(lqp>1){
        lqp--; i = leafQ[lqp]; val[i]--; i2 = i+N;
        //Now to eliminate leaf i
        eqnstack[eqp] = i; eqp++; //Push i onto stack
        for(j=0; j<3; j++) if(B[i][j] != 0.0) break;// neighbor is 4+j
        q0 = B[i][j];
        j = adj[i][j]-N;/* neighbor is j */
        val[j]--; if(val[j]==1){ leafQ[lqp] = j; lqp++; }/* new leaf? */
        for(m=0; m<3; m++) if(adj[j][m]==i2) break;
        q1 = B[j][m]; B[j][m] = 0.0;
        t = 1.0-q1*q0; t = 1.0/t;
        for(m=0; m<3; m++) B[j][m] *= t;
        for(m=0; m<D; m++){ C[j][m] += q1*C[i][m]; C[j][m] *= t; }
    }

    //Third: Solve 1-vertex tree!
    i = leafQ[0]; i2=i+N;
    for(m=0; m<D; m++) points[i2][m] = C[i][m];

    //Fourth: backsolve
    while(eqp>0){
        eqp--; i=eqnstack[eqp]; i2=i+N;
        for(j=0; j<3; j++) if(B[i][j] != 0.0) break; //neighbor is #j
        q0 = B[i][j];
        j = adj[i][j]; //Neighbor is j
        for(m = 0; m < D; m++ )
            points[i2][m] = C[i][m] + q0*points[j][m];
    }

    return;
}

ostream& operator<<(ostream& out, const TestWSConvergence& p)
{
    out<<"TestWSConvergence"<<endl;
    out<<"> adj:"<<endl;
    for(int i=0;i<3;i++){
        for(int j=0;j<p.steinerPoints;j++){
            out<<setw(4)<<p.adj[j][i];
        }
        out<<endl;
    }

    return out;
}


void TestWSConvergence::runTest1()
{
    std::vector<geomNd::Point> points;
    for(int i=0;i<4;i++)
        points.push_back(geomNd::Point(2));
    points[0][0] = 2.0; points[0][1] = 0.0;
    points[1][0] = 4.0; points[1][1] = 1.0;
    points[2][0] =-2.0; points[2][1] = 0.0;
    points[3][0] =-4.0; points[3][1] = 1.0;

    TestWSConvergence ws(points);

    ws.points[4][0] = 2.0; ws.points[4][1] = 2.0;
    ws.points[5][0] =-2.0; ws.points[5][1] = 2.0;

    ws.steinerPoints = 2;
    ws.adj[0][0] = 0; ws.adj[0][1] = 1; ws.adj[0][2] = 5;
    ws.adj[0][0] = 2; ws.adj[0][1] = 3; ws.adj[0][2] = 4;


    double tol = 0.00001;

    std::cout<<"Before: "<<std::endl;
    for(int i=0;i<6;i++){
        std::cout<<"Point "<<i<<" : "<<ws.points[i][0]<<" , "<<ws.points[i][1]<<std::endl;
    }

    double q, r;
    do{
        q = length();
        r = error();
        optimize(tol*r/N);
    }while(r>tol*q);

}
