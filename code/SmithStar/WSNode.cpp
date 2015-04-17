#include "WSNode.h"

#include <string.h>

WSNode::WSNode(WSNode *p, const int es, const int si): 
parent(p), 
depth(p->depth+1), 
edgeSplit(es), 
siteInserted(si), 
childrenAlive(0)
{
	p->childrenAlive++;
}

WSNode::WSNode(const int siteInserted_): 
parent(0), 
depth(0), 
edgeSplit(-1), 
siteInserted(siteInserted_), 
childrenAlive(0)
{
}

WSNode::~WSNode(){
	if(parent!=NULL){
		parent->childrenAlive--;
		if(parent->childrenAlive==0)
			delete parent;
	}
}

void WSNode::updatePlacementAdjacencies(PointPlacement &placement)
{
    int N = placement.N;
    //Update placement.adj so it reflects node
    const WSNode *n = this;
    int newSteiner = N;

    int kVector[N-3]; memset(kVector, 0, (N-3)*sizeof(int));
    int sVector[N-3]; memset(sVector, 0, (N-3)*sizeof(int));
    int edges[N+N-3][2];
    for(int i=0;i<N+N-3;i++) edges[i][0] = edges[i][1] = 0;

    while(n!=NULL){
        if(n->depth>2){
            kVector[n->depth-3] = n->edgeSplit;
            sVector[n->depth-3] = n->siteInserted;
        }else{
            edges[n->depth][0] = n->siteInserted;
            edges[n->depth][1] = newSteiner;
        }

        n = n->parent;
    }

    newSteiner++;

    int e=3;
    for(int i=0;i<this->depth-2;i++){
        int s = kVector[i], v2 = edges[s][1];
        edges[e][0] = sVector[i]; edges[e++][1] = newSteiner;
        edges[e][0] = v2;         edges[e++][1] = newSteiner;
        edges[s][1] = newSteiner++;
    }

    //cout<<"kVector:"<<endl;
    //for(int i=0;i<N-3;i++){
    //    cout<<kVector[i]<<" ";
    //}
    //cout<<endl;

    //cout<<"sVector:"<<endl;
    //for(int i=0;i<N-3;i++){
    //    cout<<sVector[i]<<" ";
    //}
    //cout<<endl;

    //cout<<"edges:"<<endl;
    //for(int i=0;i<N+N-3;i++)
    //    printf("%3d ",edges[i][0]);
    //cout<<endl;
    //for(int i=0;i<N+N-3;i++)
    //    printf("%3d ",edges[i][1]);
    //cout<<endl;

    int count[N-2];
    memset(count, 0, (N-2)*sizeof(int));

    for(int i=0;i<e;i++){
        int s1 = edges[i][0]-N; if(s1>=0) placement.adj[s1][count[s1]++] = edges[i][1];
        int s2 = edges[i][1]-N; if(s2>=0) placement.adj[s2][count[s2]++] = edges[i][0];
        //printf("s1=%d s2=%d\n",s1,s2);
    }
    //cout<<"adj:"<<endl;
    //for(int i=0;i<N-2;i++)
    //    printf("%4d ", placement.adj[i][0]);
    //cout<<endl;
    //for(int i=0;i<N-2;i++)
    //    printf("%4d ", placement.adj[i][1]);
    //cout<<endl;
    //for(int i=0;i<N-2;i++)
    //    printf("%4d ", placement.adj[i][2]);
    //cout<<endl;


    //Update placement.steinerPoints so it reflects node
    placement.steinerPoints = this->depth-1;
    //cout<<"steinerPoints = "<<placement.steinerPoints<<endl;
}
