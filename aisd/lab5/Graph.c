#include <stdlib.h>
#include "Graph.h"
#include "Queue.h"
#include <limits.h>
#define INF INT_MAX

//Breadth-first-search
//isExit will be a number of reachable exits.
int* BFS(Graph* G, int vi, int* isExit)   
{
	*isExit=0;

	//colors, distances, pred
	Color color[G->v];
	int dist[G->v];
	int pred[G->v];
	Queue* Q=QueueInit(G->v);

	//initializing
	for(int i=0; i<G->v; i++)
	{
		color[i]=WHITE;
		dist[i]=INF;
		pred[i]=-1;
	}

	color[vi]=GREY;
	dist[vi]=0;
	push(Q, vi);
	int u=0;
	
	//Visiting and coloring nodes
	while(pop(Q, &u)!=ERR_EMPTY)
	{
		Edge* ptr=G->vertex[u].head;
		while(ptr)
		{
			int to=ptr->to;
			if(color[to]==WHITE)
			{
				color[to]=GREY;
				dist[to]=dist[u]+ptr->w;
				pred[to]=u;
				push(Q, to);
				if(G->vertex[to].type==EXIT) *isExit=*isExit+1;
			}
			ptr=ptr->next;
		}
		color[u]=BLACK;	
	}
	return pred;
}




