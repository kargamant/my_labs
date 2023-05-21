#ifndef GRAPH_H
#define GRAPH_H

//Vertex structure
typedef struct Vertex
{
	//char* id;
	Room type;
	Edge* head; 
}Vertex;

#include "HashLib/HashTable.h"

typedef struct Graph
{
	int v;
	Table* vertex;
}Graph;

//Availiable room types in labirint
typedef enum Room
{
	ENTER,
	TRANSIT,
	EXIT
}Room;


//Edge - element of vertex list
typedef struct Edge
{
	int w;
	int to;
	struct Edge* next;
}Edge;

typedef enum Color
{
	WHITE,
	GREY,
	BLACK
}Color;

//methods
Graph* GraphInit(int v);
Vertex* VertexInit(char* id, Room type);
Edge* EdgeInit(int w, int to);
int* BFS(Graph* G, int vi, int* isExit);
int* Dejkstra(Graph* G, int from, int to, List* result);
int FindMin(Graph* G, int* dist, int* used);
void AddVert(Graph* G, char* id, Room type);
int AddEdge(Graph* G, char* from_id, char* to_id, int w);
int DelEdge(Graph* G, char* from_id, char* to_id, int w);
int DelVert(Graph* G, char* id);
int VertUpdate(Graph* G, char* id, char* new_id, Room type);
int EdgeUpdate(Graph* G, char* from_id, char* to_id, char* nfrom_id, char* nto_id, int w,int new_w);
int Show(Graph* G);

typedef enum ERR
{
	ERR_OK,
	ERR_DUPL,
	ERR_NF
}ERR;

#endif

