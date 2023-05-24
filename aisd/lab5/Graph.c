#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Graph.h"
#include "Queue.h"
#include "Heap.h"

//Init functions
Graph* GraphInit(int v)
{
	Graph* G=(Graph*)malloc(sizeof(Graph));
	G->v=0;
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
int BFS(Graph* G, char* vi_id, int* isExit, List* result, int** distances, int** predators)   
{
	int vi=Search(G->vertex, vi_id);
	if(vi==-1) return -1;
	*isExit=0;

	//colors, distances, pred
	Color color[G->vertex->msize];
	int* dist=(int*)calloc(G->vertex->msize, G->vertex->msize*sizeof(int));
	int* pred=(int*)calloc(G->vertex->msize, G->vertex->msize*sizeof(int));
	Queue* Q=QueueInit(G->vertex->msize);

	//initializing
	for(int i=0; i<G->vertex->msize; i++)
	{
		color[i]=WHITE;
		dist[i]=INF;
		pred[i]=-1;
	}

	color[vi]=GREY;
	dist[vi]=0;
	push(Q, vi);
	int u=0;
	
	push_back(result, vi);
	*isExit+=G->vertex->ks[vi].info->vertex->type==EXIT;
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
				push_back(result, to);
				push(Q, to);
				*isExit+=G->vertex->ks[to].info->vertex->type==EXIT;
			}
			
			if(dist[to]>dist[u]+ptr->w)
			{
				dist[to]=dist[u]+ptr->w;
				pred[to]=u;
			}
			ptr=ptr->next;
		}
		color[u]=BLACK;	
	}
	*distances=dist;
	*predators=pred;
	free(Q->queue);
	free(Q);
	return ERR_OK;
}

//From entrance to exit
int Dejkstra(Graph* G, char* from_id, char* to_id, List* result)
{
	int from=Search(G->vertex, from_id);
	int to=Search(G->vertex, to_id);
	if(from==-1 || to==-1) return -1;
	int* dist=(int*)calloc(G->vertex->msize, G->vertex->msize*sizeof(int));
	int* pred=(int*)calloc(G->vertex->msize, G->vertex->msize*sizeof(int));
	int* used=(int*)calloc(G->vertex->msize, G->vertex->msize*sizeof(int));
	Heap* H=HeapInit(G->v);
	int where[G->vertex->msize];
	for(int i=0; i<G->vertex->msize; i++)
	{
		dist[i]=INF;
		used[i]=0;
		pred[i]=-1;
	}
	for(int i=0; i<G->vertex->msize; i++)
	{
		if(G->vertex->ks[i].busy==BUSY) 
		{
			pushH(H, i, dist, where);
		}
	}
	dist[from]=0;
	SiftUp(H, where[from], dist, where);

	while(H->size!=0)
	{
		int u=ExtrMin(H, dist, where);
		if(dist[u]==INF) break;
		used[u]=1;
		//push_back(result, u);
		//u=FindMin(G, dist, used);
		Edge* pk=G->vertex->ks[u].info->vertex->head;
		while(pk)
		{
			int pkto=pk->to;
			/*Edge* upkto=G->vertex->ks[u].info->vertex->head;
			while(upkto)
			{
				if(upkto->to==pkto) break;
				upkto=upkto->next;
			}*/
			if(dist[pkto]>dist[u]+pk->w)
			{
				dist[pkto]=dist[u]+pk->w;
				pred[pkto]=u;
				if(where[pkto]!=-1) SiftUp(H, where[pkto], dist, where);
			}
			pk=pk->next;
		}
	}

	int k=to;
	while(k!=-1)
	{
		push_back(result, k);
		//used[k]=1;
		//printf("k: %d\n", k);
		k=pred[k];
	}
	int shortest=dist[to];
	free(dist);
	free(pred);
	free(used);
	eraseH(H);	
	return shortest;
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
	if(vi!=-1) return -1;
	//To be made sooner
	//if(G->vertex->msize<=G->v) G->vertex=rebuild(G->vertex);
	int j=add(G->vertex, id, type);
	G->v+=1;
	return j;
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
	G->vertex->ks[from].info->vertex->head=E;
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
	return ERR_OK;
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
}

int VertUpdate(Graph* G, char* id, char* new_id, Room type)
{
	if(!strcmp(id, new_id))
	{
		int vo=Search(G->vertex, id);
		if(vo==-1) return -1;
		G->vertex->ks[vo].info->vertex->type=type;
		return ERR_OK;
	}
	int vo=Search(G->vertex, id);
	
	if(vo==-1) return -1;
	else
	{
		int j=AddVert(G, new_id, type);
		if(j==-1) return -2;
		vo=Search(G->vertex, id);
		G->vertex->ks[j].info->vertex->head=G->vertex->ks[vo].info->vertex->head;
		G->vertex->ks[vo].info->vertex->head=NULL;
		int res=Del(G->vertex, id);
		for(KeySpace* ptr=G->vertex->ks; ptr-G->vertex->ks<G->vertex->msize; ++ptr)
		{
			if(ptr->busy==BUSY)
			{
				Edge* pk=ptr->info->vertex->head;
					
				while(pk)
				{
					if(pk->to==res) pk->to=j;
					pk=pk->next;
				}
			}
		}
		//DelVert(G, id);
		return ERR_OK;
	}
}

int EdgeUpdate(Graph* G, char* from_id, char* to_id, char* nfrom_id, char* nto_id, int w,int new_w)
{
	int from=Search(G->vertex, from_id);
	int to=Search(G->vertex, to_id);
	int nfrom=Search(G->vertex, nfrom_id);
	int nto=Search(G->vertex, nto_id);
	
	if(from==-1 || to==-1 || nfrom==-1 || nto==-1) return ERR_NF;
	Vertex* from_V=G->vertex->ks[from].info->vertex;
	Edge* pk=from_V->head;
	while(pk)
	{
		if(pk->to==to && pk->w==w) break;
		pk=pk->next;
	}
	if(!pk) return ERR_NF;
	if(from==nfrom)
	{
		pk->to=nto;
		pk->w=new_w;
	}
	else
	{
		AddEdge(G, nfrom_id, nto_id, new_w);
		DelEdge(G, from_id, to_id, w);
	}
	return ERR_OK;
}

int Show(Graph* G)
{
	printf("Amount of vertices: %d\n", G->v);
	if(G->v==0) return ERR_OK;
	Table* t=G->vertex;
	printf("id | Room type | (weight, id)\n");
	for(int i=0; i<t->msize; i++)
	{
		if(t->ks[i].busy==BUSY)
		{
			printf("\"%s\" | ", t->ks[i].key);
			switch(t->ks[i].info->vertex->type)
			{
				case ENTER:
					printf("ENTER | ");
					break;
				case TRANSIT:
					printf("TRANSIT | ");
					break;
				case EXIT:
					printf("EXIT | ");
					break;
			}
			Edge* pk=t->ks[i].info->vertex->head;
			while(pk)
			{
				printf("(%d, \"%s\") ", pk->w, G->vertex->ks[pk->to].key);
				pk=pk->next;
			}
			printf("\n");
		}
	}
	return ERR_OK;
}

int ImageGenerate(Graph* G, char* fn, Kraskal* edges, int e, int* vertecies, int v)
{
	FILE* fd=fopen(fn, "w+");
	fclose(fd);
	fd=fopen(fn, "a+");
	fprintf(fd, "digraph {\n");
	for(int i=0; i<G->vertex->msize; i++)
	{
		if(G->vertex->ks[i].busy==BUSY)
		{
			int is_colored=0;
			if(vertecies)
			{
				for(int j=0; j<v; j++)
				{
					if(i==vertecies[j])
					{
						is_colored=1;
						break;
					}
				}
			}
			if(is_colored) fprintf(fd, "%s[color=\"red\"]\n", G->vertex->ks[i].key);
			else fprintf(fd, "%s\n", G->vertex->ks[i].key);
			Edge* pk=G->vertex->ks[i].info->vertex->head;
			while(pk)
			{
				is_colored=0;
				if(edges)
				{
					for(int j=0; j<e; j++)
					{
						if(edges[j].from==i && edges[j].to==pk->to && edges[j].w==pk->w)
						{
							is_colored=1;
							break;
						}
					}
				}
				if(is_colored) fprintf(fd, "%s->%s [label=%d, weight=%d, color=\"red\"]\n", G->vertex->ks[i].key, G->vertex->ks[pk->to].key, pk->w, pk->w);
				else fprintf(fd, "%s->%s [label=%d, weight=%d]\n", G->vertex->ks[i].key, G->vertex->ks[pk->to].key, pk->w, pk->w);
				pk=pk->next;
			}
		}
	}
	fprintf(fd, "}");
	fclose(fd);
}

void fimport(Graph* G, char* fn)
{
	FILE* fd=fopen(fn, "r+");
	int n=0;
	fscanf(fd, "%d\n", &n);
	for(int i=0; i<n; i++)
	{
		char* id=(char*)malloc(2*sizeof(char));
		int type=0;
		fscanf(fd, "%s %d\n", id, &type);
		//id=(char*)realloc(id, strlen(id)*sizeof(char));
		AddVert(G, id, type);
	}
	while(!feof(fd))
	{
		char* from_id=(char*)malloc(2*sizeof(char));
		char* to_id=(char*)malloc(2*sizeof(char));
		int w=0;
		fscanf(fd, "%s %s %d\n", from_id, to_id, &w);
		//from_id=(char*)realloc(from_id, strlen(from_id)*sizeof(char));
		//to_id=(char*)realloc(to_id, strlen(to_id)*sizeof(char));
		AddEdge(G, from_id, to_id, w);
		free(from_id);
		free(to_id);
	}
	fclose(fd);
}

int KraskalCmp(Kraskal* a, Kraskal* b)
{
	return a->w-b->w;
}

Kraskal* Kraskala(Graph* G, int* ms_size)
{
	Kraskal* edges=(Kraskal*)malloc(sizeof(Kraskal));
	int* color=(int*)malloc(G->vertex->msize*sizeof(int));
	Kraskal* MST=(Kraskal*)malloc(sizeof(Kraskal));
	int mst=0;
	int n=0;
	for(int i=0; i<G->vertex->msize; i++)
	{
		color[i]=i;
		Edge* pk=G->vertex->ks[i].info->vertex->head;
		while(pk)
		{
			edges[n].from=i;
			edges[n].to=pk->to;
			edges[n].w=pk->w;
			n++;
			if(pk->next) edges=(Kraskal*)realloc(edges, (n+1)*sizeof(Kraskal));
			pk=pk->next;
		}
		edges=(Kraskal*)realloc(edges, (n+1)*sizeof(Kraskal));
	}
	qsort(edges, n, sizeof(Kraskal), (int(*)(const void*, const void*))KraskalCmp);	
	for(int i=0; i<n; i++)
	{
		Kraskal* e=edges+i;
		if(color[e->from]!=color[e->to])
		{
			int color_from=color[e->from];
			int color_to=color[e->to];
			for(int j=0; j<G->vertex->msize; j++)
			{
				if(color[j]==color_from) color[j]=color_to;
			}
			printf("edge: (\"%s\", \"%s\", w%d)\n", G->vertex->ks[e->from].key, G->vertex->ks[e->to].key, e->w);
			MST[mst]=*e;
			mst++;
			if(i<n-1) MST=(Kraskal*)realloc(MST, (mst+1)*sizeof(Kraskal));
		}
	}
	free(edges);
	free(color);
	*ms_size=mst;
	return MST;
}
/*
Graph* Raif_Saitgaliev(Graph* G)
{
	Vertex* entres=(Vertex*)malloc(1*sizeof(Vertex));
	int ens=0;
	Vertex* exits=(Vertex*)malloc(1*sizeof(Vertex));
	int exs=0;
	for(int i=0; i<G->vertex->msize; i++)
	{
		if(G->vertex->ks[i].busy==BUSY && G->vertex->ks[i].info->vertex->type==ENTER)
		{
			entres[ens]=*G->vertex->ks[i].info->vertex;
			ens++;
			if(i!=G->vertex->msize-1) entres=(Vertex*)realloc(entres, (ens+1)*sizeof(Vertex));
		}
		
		if(G->vertex->ks[i].busy==BUSY && G->vertex->ks[i].info->vertex->type==EXIT)
		{
			entres[exs]=*G->vertex->ks[i].info->vertex;
			exs++;
			if(i!=G->vertex->msize-1) exits=(Vertex*)realloc(exits, (exs+1)*sizeof(Vertex));
		}
	}
	for(int i=0; i<ens; i++)
	{
		List* result=L_init();

	}
}*/

