#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcs.h"
#include "Graph.h"

//string parsing from lab2
char* enter()
{
	char buf[81]={0};
	char* ptr=(char*)malloc(1);
	*ptr='\0';
	int n, len=0;
	do
	{
		n=scanf("%80[^\n]", buf);
		if(n<0) 
		{
			free(ptr);
			ptr=NULL;
			break;
		}
		else if(n==0) 
		{
			scanf("%*c");
		}
		else 
		{
			len+=strlen(buf);
			ptr=(char*)realloc(ptr, len+1);
			strcat(ptr, buf);

		}
	}while(n>0);
	return ptr;
}

//file parsing version
char* enterf(FILE* fd)
{
	char buf[81]={0};
	char* ptr=(char*)malloc(1);
	*ptr='\0';
	int n, len=0;
	do
	{
		n=fscanf(fd, "%80[^\n]", buf);
		if(n<0) 
		{
			free(ptr);
			ptr=NULL;
			continue;
		}
		else if(n==0) fscanf(fd, "%*c");
		else 
		{
			len+=strlen(buf);
			ptr=(char*)realloc(ptr, len+1);
			strcat(ptr, buf);

		}
	}while(n>0);
	return ptr;
}

//int parsing from file
int GetIntf(FILE* fd)
{
	int in=0, data;
	in=fscanf(fd, "%d\n", &data);
	if(in==EOF) 
	{
		return -1;
	}
	else if(in==0)
	{
		printf("Error. Wrong data.\n");
		return -1;
	}
	else if(data<0)
	{
		printf("Error. Key or msize must be positive.\n");
		return -1;
	}
	else return data;
}

//Correct input of integer from lab1
int getInt(int* n)
{
	int input;
	do
	{
		input=scanf("%d", n);
		if(input==EOF) return 1;
		else if(input==0)
		{
			printf("Error, wrong data. Try again\n");
			scanf("%*[^\n]"); 
			continue;
		}
		else
		{
			scanf("%*c");
			return 0;
		}
	}while(input==0 || *n<0);
}


int getLL(long long int* n)
{
	int input;
	do
	{
		input=scanf("%lld", n);
		if(input==EOF) return 1;
		else if(input==0)
		{
			printf("Error, wrong data. Try again\n");
			scanf("%*[^\n]"); 
			continue;
		}
		else
		{
			scanf("%*c");
			return 0;
		}
	}while(input==0 || *n<0);
}

int getDb(double* n)
{
	int input;
	do
	{
		input=scanf("%lf", n);
		if(input==EOF) return 1;
		else if(input==0)
		{
			printf("Error, wrong data. Try again\n");
			scanf("%*[^\n]"); 
			continue;
		}
		else
		{
			scanf("%*c");
			return 0;
		}
	}while(input==0 || *n<0);
}

int fcheck(char* fnd)
{
	if(!fnd) 
	{
		free(fnd);
		fnd=NULL;
		return CERR_EOF;
	}
	if(*fnd==0) 
	{
		free(fnd);
		fnd=NULL;
		fnd=enter();
	}
	return CERR_OK;
}

//MVC paradigm
//------------------------
//Main Controller function
int console(int p, Graph* G)
{	
	int (*view[])(Graph*)={Showv, AddVertv, AddEdgev, DelVertv, DelEdgev, VertUpdatev, EdgeUpdatev, BFSv, Dejkstrav, Importv, Kraskalav};
	return view[p](G);	
}


//View functions 



int Kraskalav(Graph* G)
{
	int mst=0;
	Kraskal* MST=Kraskala(G, &mst);
	

	ImageGenerate(G, "labirint.dot", MST, mst, NULL, 0);
	system("python3 viz.py");
	system("gthumb labirint.png");
	free(MST);
	return EndView();
}

int Importv(Graph* G)
{
	printf("Enter filename: ");
	char* fn=enter();
	if(fcheck(fn)==CERR_EOF) return CERR_EOF;

	fimport(G, fn);
	free(fn);
	return EndView();
}

int Dejkstrav(Graph* G)
{
	printf("Enter id of starting room: ");
	char* from_id=enter();
	if(!from_id) return CERR_EOF;

	printf("Enter to_id room: ");
	char* to_id=enter();
	if(!to_id) return CERR_EOF;

	List* res=L_init();
	int length=Dejkstra(G, from_id, to_id, res);
	free(from_id);
	free(to_id);
	if(length==-1) printf("Error. From or to id is not in graph.\n");
	else
	{
		printf("Length of the journey: %d\n", length);
		Node* pk=res->head;
		printf("Path: ");
		//searching for edges

		int n=0;
		while(pk)
		{
			Node* next=pk->next;
			n++;
			//printf("\"%s\" ", G->vertex->ks[pk->data].key);
			pk=next;
		}
		char* reverse[n];
		pk=res->head;
		int t=1;
		while(pk)
		{
			Node* next=pk->next;
			reverse[n-t]=G->vertex->ks[pk->data].key;
			t++;
			free(pk);
			pk=next;
		}
	
		//preparing to draw
		int draw[n];
		for(int i=0; i<n; i++) 
		{
			printf("\"%s\" ", reverse[i]);
			draw[i]=Search(G->vertex, reverse[i]);
		}

		Kraskal* path=(Kraskal*)malloc(sizeof(Kraskal));
		int pth=0;
		for(int j=0; j<n-1; j++)
		{
			int minw=INF;
			Edge* pk=G->vertex->ks[draw[j]].info->vertex->head;
			while(pk)
			{
				if(pk->w < minw && pk->to==draw[j+1]) 
				{
					minw=pk->w;
				}
				pk=pk->next;
			}
			path[pth].from=draw[j];
			path[pth].to=draw[j+1];
			path[pth].w=minw;
			pth++;
			if(j<n-2) path=(Kraskal*)realloc(path, (pth+1)*sizeof(Kraskal));
		}
		printf("\n");
		for(int i=0; i<pth; i++)
		{
			printf("edge: (%d, %d, w%d)\n", path[i].from, path[i].to, path[i].w);
		}
		if(length==INF) ImageGenerate(G, "labirint.dot", NULL, 0, NULL, 0);
		else ImageGenerate(G, "labirint.dot", path, pth, draw, n);
		system("python3 viz.py");
		system("gthumb labirint.png");
		free(path);
	}
	free(res);
	return EndView();
}

int BFSv(Graph* G)
{
	int isExit=0;
	printf("Enter id of starting room: ");
	char* vi_id=enter();
	if(!vi_id) return CERR_EOF;

	List* path=L_init();
	int* distances=NULL;
	int* predators=NULL;
	int t=BFS(G, vi_id, &isExit, path, &distances, &predators);
	if(t==-1) goto er;
	printf("path: ");
	Node* pt=path->head;
	while(pt)
	{
		Node* next=pt->next;
		printf("\"%s\" ", G->vertex->ks[pt->data].key);
		pt=next;
	}
	printf("\n");

	printf("distances: ");
	pt=path->head;
	while(pt)
	{
		Node* next=pt->next;
		printf("%d ", distances[pt->data]);
		free(pt);
		pt=next;
	}
	printf("\n");

	printf("Exits reachable from \"%s\": %d\n", vi_id, isExit);
	if(G->vertex->ks[Search(G->vertex, vi_id)].info->vertex->type!=ENTER) printf("\nWarning. Consider that start vertex of BFS was not a room of type ENTER, as it supposed to be.\n\n");
	er:
		printf("Error. Id you have entered is not in graph.\n");
	free(path);
	free(vi_id);
	free(distances);
	free(predators);
	return EndView();	
}

int Showv(Graph* G)
{	
	Show(G);
	ImageGenerate(G, "labirint.dot", NULL, 0, NULL, 0);
	system("python3 viz.py");
	system("gthumb labirint.png");
	return EndView();
}

int AddVertv(Graph* G)
{
	printf("Enter id of new room: ");
	char* id=enter();
	if(!id) return CERR_EOF;
	
	printf("Now choose a room type:\n");
	printf("0 - ENTER\n");
	printf("1 - TRANSIT\n");
	printf("2 - EXIT\n");
	
	int type=0;
	do
	{
		int input=getInt(&type);
		if(input) return CERR_EOF;
		if(type<0 || type>2) printf("Error. room type can only be chosen of three types above: 0, 1, 2\n");
	}while(type<0 || type>2);
	
	int result=AddVert(G, id, type);
	if(result==-1) printf("Error. Room with this id already exists.\n");
	else printf("Vertex with id \"%s\" was successfully added to graph.\n", id);
	
	return EndView();	
}

int AddEdgev(Graph* G)
{
	printf("Enter from_id room: ");
	char* from_id=enter();
	if(!from_id) return CERR_EOF;
	
	printf("Enter to_id room: ");
	char* to_id=enter();
	if(!to_id) return CERR_EOF;

	printf("Enter weight of edge: ");
	int w=0;
	int input=getInt(&w);
	if(input) return CERR_EOF;

	int result=AddEdge(G, from_id, to_id, w);
	if(result==ERR_NF) printf("Error. No vertecis with from or to id.\n");
	else printf("Edge beetween \"%s\" and \"%s\" was succesfully added.\n", from_id, to_id);

	free(from_id);
	free(to_id);

	return EndView();
}

int DelVertv(Graph* G)
{
	printf("Enter id of room: ");
	char* id=enter();
	if(!id) return CERR_EOF;

	int result=DelVert(G, id);
	if(result==ERR_NF) printf("Error. No vertex with this id found.\n");
	else printf("Vertex with id \"%s\" was successfully deleted.\n", id);

	free(id);
	return EndView();
}

int DelEdgev(Graph* G)
{
	printf("Enter from_id room: ");
	char* from_id=enter();
	if(!from_id) return CERR_EOF;
	
	printf("Enter to_id room: ");
	char* to_id=enter();
	if(!to_id) return CERR_EOF;

	printf("Specify weight of deleting edge: ");
	int w=0;
	int input=getInt(&w);
	if(input) return CERR_EOF;

	int result=DelEdge(G, from_id, to_id, w);
	if(result==ERR_NF) printf("Error. No vertecies with from or to id found.\n");
	else printf("Edge beetween \"%s\" and \"%s\" was succesfully deleted.\n", from_id, to_id);

	free(from_id);
	free(to_id);
	return EndView();
}

int VertUpdatev(Graph* G)
{
	printf("Enter id of room: ");
	char* id=enter();
	if(!id) return CERR_EOF;
	
	printf("Enter new id room: ");
	char* new_id=enter();
	if(!new_id) return CERR_EOF;
	
	printf("Now choose a room new type:\n");
	printf("0 - ENTER\n");
	printf("1 - TRANSIT\n");
	printf("2 - EXIT\n");
	
	int type=0;
	do
	{
		int input=getInt(&type);
		if(input) return CERR_EOF;
		if(type<0 || type>2) printf("Error. room type can only be chosen of three types above: 0, 1, 2\n");
	}while(type<0 || type>2);

	int result=VertUpdate(G, id, new_id, type);
	if(result==-1) 
	{
		printf("Error. No vertex with this id in graph.\n");
		free(new_id);
	}
	else if(result==-2)
	{
		printf("Error. Vertex with new_id is already in graph.\n");
		free(new_id);
	}
	else printf("Data of this room was succesfully updated.\n");

	free(id);
	return EndView();
}

int EdgeUpdatev(Graph* G)
{
	printf("Enter from_id room: ");
	char* from_id=enter();
	if(!from_id) return CERR_EOF;
	
	printf("Enter to_id room: ");
	char* to_id=enter();
	if(!to_id) return CERR_EOF;

	printf("Enter new from_id room: ");
	char* nfrom_id=enter();
	if(!nfrom_id) return CERR_EOF;
	
	printf("Enter new to_id room: ");
	char* nto_id=enter();
	if(!nto_id) return CERR_EOF;
	
	printf("Enter weight of edge: ");
	int w=0;
	int input=getInt(&w);
	if(input) return CERR_EOF;

	
	printf("Enter new weight of edge: ");
	int new_w=0;
	input=getInt(&new_w);
	if(input) return CERR_EOF;

	int result=EdgeUpdate(G, from_id, to_id, nfrom_id, nto_id, w, new_w);
	if(result==ERR_NF) printf("Error. Some of ids missing in graph.\n");	
	else printf("Information about this edge was successfully updated.\n");
	
	free(from_id);
	free(to_id);
	free(nfrom_id);
	free(nto_id);

	return EndView();
}

//Function that ends a view function
int EndView()
{
	int i=0;
	printf("Input any value to continue or EOF to stop.\n");
	int cont=scanf("%d", &i);
	scanf("%*[^\n]");
	if(cont==EOF) return CERR_EOF;
	return CERR_OK;
}
