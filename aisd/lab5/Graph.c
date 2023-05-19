#include <stdlib.h>
#include "Graph.h"
#include "Queue.h"
#include <limits.h>
#define INF INT_MAX


//Init functions
Graph* GraphInit(int v)
{
	Graph* G=(Graph*)malloc(sizeof(Graph));
	G->v=v;
	G->vertex=(Vertex*)malloc(v*sizeof(Vertex));
	return G;
}

Vertex* VertexInit(char* id, Room type)
{
	Vertex* V=(Vertex*)malloc(sizeof(Vertex));
	V->id=id;
	V->type=type;
	V->head=NULL;
	return V;
}

Edge* EdgeInit(int w, int to)
{
	Edge* E=(Edge*)malloc(sizeof(Edge));
	E->w=w;
	E->to=to;
	E->next=NULL;
	return E;
}

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

//From entrance to exit
int* Dejkstra(Graph* G, int from, int to, List* result)
{
	int dist[G->v];
	int pred[G->v];
	int used[G->v];
	for(int i=0; i<G->v; i++)
	{
		dist[i]=INF;
		used[i]=0;
		pred[i]=-1;
	}
	dist[from]=0;

	int u=0;
	while(u!=0 || !used[0])
	{
		u=FindMin(G, dist, used);
		used[u]=1;
		Edge* pk=G->vertex[u].head;
		while(pk)
		{
			if(dist[pk->to]>dist[u]+G->vertex[u].w)
			{
				dist[pk->to]=dist[u]+G->vertex[u].w;
				pred[pk->to]=u;
			}
			pk=pk->next;
		}
	}
	int k=to;
	while(k!=-1)
	{
		push(result, k);
		k=pred[k];
	}
	return dist;
}

int FindMin(Graph* G, int* dist, int* used)
{
	int mini=0;
	for(int i=0; i<G->v; i++)
	{
		if(used[i]) continue;
		if(dist[i]<dist[mini]) mini=i;
	}
	return mini;
}

//Make an id check
int AddNode(Graph* G, char* id, Room type)
{
	Vertex* V=VertexInit(id, type);
	G->vertex=(Vertex*)realloc((G->v+1)*sizeof(Vertex));
	G->vertex[G->v]=V;
	G->v+=1;
}

//Make a check if such edge exist. If exist we can simply change the weight of it
int AddEdge(Graph* G, int from, int to, int w)
{
	Edge* E=EdgeInit(w, to);
	E->next=G->vertex[from].head;
	G->vertex[from].head=E;
}


