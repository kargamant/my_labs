#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "funcs.h"
#include "menue.h"

int main()
{
	Graph* G=GraphInit(10);

	//options for menue
	const char* options[]={
		"Show graph as adjacency list\n",
		"Add vertex\n",
		"Add edge\n",
		"Delete vertex\n",
		"Delete edge\n",
		"Change data of vertex\n",
		"Change data of edge\n",
		"Make Breadth-first search(BFS)\n",
		//"Find minimum path beetween vertices(Dejkstra)\n",
	};
	
	system("clear");
	//main cycle
	do
	{
		int p=menue(options, 8);
		if(p==9) break;
		int res=console(p, G);
		if(res==CERR_EOF) break;
		system("clear");
	}while(1);
	printf("Program stopped.\n");

	erased(G->vertex);
	free(G);
	return 0;
}
