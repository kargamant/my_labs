#ifndef GRAPH_H
#define GRAPH_H

//Edge - element of vertex list
typedef struct Edge
{
	int w;
	int to;
	struct Edge* next;
}Edge;

//Availiable room types in labirint
typedef enum Room
{
	ENTER,
	TRANSIT,
	EXIT
}Room;

//Vertex structure
typedef struct Vertex
{
	char* id;
	Room type;
	Edge* head; 
}Verex;

typedef struct Graph
{
	int v;
	Vertex* vertex;
}Graph;

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
void AddNode(Graph* G, char* id, Room type);
#endif

