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

#endif
