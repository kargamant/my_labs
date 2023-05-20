#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "Queue.h"
#include <limits.h>
#define INF INT_MAX


//Init functions
Graph* GraphInit(int v)
{
	Graph* G=(Graph*)malloc(sizeof(Graph));
	G->v=v;
	G->vertex=create();
	New(v, G->vertex);
	return G;
}

Vertex* VertexInit(Room type)
{
	Vertex* V=(Vertex*)malloc(sizeof(Vertex));
	//V->id=id;
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
int* BFS(Graph* G, char* vi_id, int* isExit)   
{
	int vi=Search(G->vertex, vi_id);
	if(vi==-1) return NULL;
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
		Edge* ptr=G->vertex->ks[u].info->vertex->head;
		while(ptr)
		{
			int to=ptr->to;
			if(color[to]==WHITE)
			{
				color[to]=GREY;
				dist[to]=dist[u]+ptr->w;
				pred[to]=u;
				push(Q, to);
				if(G->vertex->ks[to].info->vertex->type==EXIT) *isExit=*isExit+1;
			}
			ptr=ptr->next;
		}
		color[u]=BLACK;	
	}
	return pred;
}

//From entrance to exit
int* Dejkstra(Graph* G, char* from_id, char* to_id, List* result)
{
	int from=Search(G->vertex, from_id);
	int to=Search(G->vertex, to_id);
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
		Edge* pk=G->vertex->ks[u].info->vertex->head;
		while(pk)
		{
			int pkto=pk->to;
			if(dist[pkto]>dist[u]+G->vertex[u].w)
			{
				dist[pkto]=dist[u]+G->vertex[u].w;
				pred[pkto]=u;
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

int AddVert(Graph* G, char* id, Room type)
{
	int vi=Search(G->vertex, id);
	if(vi!=-1) return ERR_DUPL;
	if(G->vertex->msize<=G->v) G->vertex=rebuild(G->vertex);
	add(G->vertex, id, type);
	G->v+=1;
	return ERR_OK;
//	for(int i=0; i<G->v; i++)
//	{
//		if(!strcmp(G->vertex[i].id, id)) return ERR_DUPL;
//	}
//	Vertex* V=VertexInit(id, type);
//	G->vertex=(Vertex*)realloc((G->v+1)*sizeof(Vertex));
//	G->vertex[G->v]=V;
//	G->v+=1;
//	return ERR_OK;
}

int AddEdge(Graph* G, char* from_id, char* to_id, int w)
{
	int from=Search(G->vertex, from_id);
	int to=Search(G->vertex, to_id);
	if(to==-1 || from==-1) return ERR_NF;

	Edge* E=EdgeInit(w, to);
	E->next=G->vertex->ks[from].info->vertex->head;
	G->vertex[from].head=E;
	return ERR_OK;
}

int DelEdge(Graph* G, char* from_id, char* to_id, int w)
{
	int from=Search(G->vertex, from_id);
	int to=Search(G->vertex, to_id);
	
	if(from==-1 || to==-1) return ERR_NF;
	Edge* ptr=G->vertex->ks[from].info->vertex->head;
	Edge* prev=G->vertex->ks[from].info->vertex->head;
	while(ptr)
	{
		Edge* next=ptr->next;
		if(ptr->to==to && ptr->w==w)
		{
			if(ptr==G->vertex->ks[from].info->vertex->head)
			{
				G->vertex->ks[from].info->vertex->head=next;
				free(ptr);
			}
			else
			{
				prev->next=next;
				free(ptr);
			}
		}
		else prev=ptr;
		ptr=next;
	}
}

int DelVert(Graph* G, char* id)
{
	int res=Del(G->vertex, id);
	if(res==-1) return ERR_NF;
	else
	{
		G->v-=1;
		for(KeySpace* ptr=G->vertex->ks; ptr-G->vertex->ks<G->vertex->msize; ++ptr)
		{
			if(ptr->busy==BUSY)
			{
				Edge* pk=ptr->info->vertex->head;
				Edge* prev=ptr->info->vertex->head;
				
				while(pk)
				{
					Edge* next=pk->next;
					if(pk->to==res)
					{
						if(pk!=ptr->info->vertex->head)
						{
							prev->next=next;
							free(pk);
						}
						else 
						{
							ptr->info->vertex->head=next;
							free(pk);
						}
					}
					else prev=pk;
					pk=next;
				}
			}
		}
	}
	return ERR_OK;
	/*Vertex* V=NULL;
	for(int i=0; i<G->v; i++)
	{
		if(!strcmp(G->vertex[i].id, id)) 
		{
			V=G->vertex[i];
			break;
		}
	}
	if(!V) return ERR_NF;
	Edge* ptr=V->head;
	while(ptr)
	{
		Edge* next=ptr->next;
		free(ptr);
		ptr=next;
	}
	free(V->id);
	free(V);
	G->v-=1;
	return ERR_OK;*/
}

