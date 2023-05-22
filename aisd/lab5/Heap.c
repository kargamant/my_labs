#include <stdlib.h>
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

void SiftUp(Heap* H, int v, int* dist)
{
	while(v>0 && DistCmp(H->nodes[v], H->nodes[(v-1)/2], dist))
	{
		int tmp=H->nodes[(v-1)/2];
		H->nodes[(v-1)/2]=H->nodes[v];
		H->nodes[v]=tmp;
		v=(v-1)/2;
	}
}

void SiftDown(Heap* H, int v, int* dist)
{
	while(2*v+1<H->size && (DistCmp(H->nodes[2*v+1], H->nodes[v], dist) || DistCmp(H->nodes[2*v+2], H->nodes[v], dist)))
	{
		int lch=2*v+1;
		int rch=2*v+2;
		int mc=lch;
		if(rch<H->size && DistCmp(H->nodes[rch], H->nodes[lch], dist)) mc=rch;
		int tmp=H->nodes[mc];
		H->nodes[mc]=H->nodes[v];
		H->nodes[v]=tmp;
		v=mc;
	}
}

void pushH(Heap* H, int d, int* dist)
{
	H->nodes[H->size]=d;
	SiftUp(H, H->size, dist);
	H->size+=1;
}

int ExtrMin(Heap* H, int* dist)
{
	int mini=H->nodes[0];
	H->nodes[0]=H->nodes[H->size-1];
	H->size-=1;
	SiftDown(H, 0, dist);
	return mini;
}




