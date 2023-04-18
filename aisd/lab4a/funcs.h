#ifndef FUNCS_H
#define FUNCS_H
#include <stdio.h>
#include "BinTree.h"

char* enter();
char* enterf(FILE* fd);
int GetIntf(FILE* fd);
int getInt(int* n);
int console(int p, Node* root);

//view functions
int Importv(Node* root);
int Traverv(Node* root);
int Addv(Node* root);
int Delv(Node* root);
int Searchv(Node* root);
int Maxv(Node* root);
int Minv(Node* root);
int Showv(Node* root);
int Generv(Node* root);
int EndView();
typedef enum CERR
{
	CERR_OK,
	CERR_EOF
}CERR;

#endif
