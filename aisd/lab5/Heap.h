#ifndef HEAP_H
#define HEAP_H

typedef struct Heap
{
	int size;
	int* nodes;
}Heap;

Heap* HeapInit(int size);
int DistCmp(int vi, int vj, int* dist);
void SiftUp(Heap* H, int v, int* dist, int* where);
void SiftDown(Heap* H, int v, int* dist, int* where);
void pushH(Heap* H, int d, int* dist, int* where);
int ExtrMin(Heap* H, int* dist, int* where);
void eraseH(Heap* H);
#endif
