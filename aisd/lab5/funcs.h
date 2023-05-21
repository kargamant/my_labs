#ifndef FUNCS_H
#define FUNCS_H
#include <stdio.h>
#include "Graph.h"

char* enter();
char* enterf(FILE* fd);
int GetIntf(FILE* fd);
int getInt(int* n);
int console(int p, Graph* G);

//view functions
//void GenerateImage(Graph* G);
//int Importv(Graph* G);
int BFSv(Graph* G);
//int Dejkstrav(Graph* G);
int AddVertv(Graph* G);
int AddEdgev(Graph* G);
int DelVertv(Graph* G);
int DelEdgev(Graph* G);
int VertUpdatev(Graph* G);
int EdgeUpdatev(Graph* G);
int Showv(Graph* G);
int EndView();
typedef enum CERR
{
	CERR_OK,
	CERR_EOF
}CERR;

#endif
