#include <stdlib.h>
#include <stdio.h>
#include "Heap.h"

Heap* HeapInit(int size)
{
	Heap* H=(Heap*)malloc(sizeof(Heap));
	H->size=0;
	H->nodes=(int*)malloc(size*sizeof(int));
	return H;
}

int DistCmp(int vi, int vj, int* dist)
{
	return dist[vi]<dist[vj];
}

void SiftUp(Heap* H, int v, int* dist, int* where)
{
	while(v>0 && v<H->size && DistCmp(H->nodes[v], H->nodes[(v-1)/2], dist))
	{
		//if(v==0) break;
		int tmp=H->nodes[(v-1)/2];
		H->nodes[(v-1)/2]=H->nodes[v];
		H->nodes[v]=tmp;
		where[H->nodes[(v-1)/2]]=(v-1)/2;
		where[H->nodes[v]]=v;
		//printf("v: %d\n(v-1)/2: %d\n", v, (v-1)/2);
		//if((v-1)/2==0) break;
		v=(v-1)/2;
		//printf("v after: %d\n", v);
	}
}

void SiftDown(Heap* H, int v, int* dist, int* where)
{
	while(2*v+1<H->size)
	{
		int lch=2*v+1;
		int rch=2*v+2;
		int mc=lch;
		if(rch<H->size && DistCmp(H->nodes[rch], H->nodes[lch], dist)) mc=rch;
		if(DistCmp(H->nodes[v], H->nodes[mc], dist) || dist[H->nodes[v]]==dist[H->nodes[mc]]) break;
		int tmp=H->nodes[mc];
		H->nodes[mc]=H->nodes[v];
		H->nodes[v]=tmp;
		where[H->nodes[mc]]=mc;
		where[H->nodes[v]]=v;
		v=mc;
	}
}

void pushH(Heap* H, int d, int* dist, int* where)
{
	H->nodes[H->size]=d;
	where[d]=H->size;
	H->size+=1;
	SiftUp(H, H->size, dist, where);
}

int ExtrMin(Heap* H, int* dist, int* where)
{
	int mini=H->nodes[0];
	H->nodes[0]=H->nodes[H->size-1];
	where[H->nodes[0]]=0;
	where[H->nodes[H->size-1]]=H->size-1;
	where[mini]=-1;
	H->size-=1;
	SiftDown(H, 0, dist, where);
	return mini;
}

void eraseH(Heap* H)
{
	free(H->nodes);
	free(H);
}


