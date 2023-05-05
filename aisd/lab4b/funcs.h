#ifndef FUNCS_H
#define FUNCS_H
#include <stdio.h>
#include "Btree.h"

char* enter();
char* enterf(FILE* fd);
int GetIntf(FILE* fd);
int getInt(int* n);
int console(int p, Btree* tr);

//view functions
int Importv(Btree* tr);
int Traverv(Btree* tr);
int Addv(Btree* tr);
int Delv(Btree* tr);
int Searchv(Btree* tr);
int Maxv(Btree* tr);
int Minv(Btree* tr);
int Showv(Btree* tr);
//int Generv(Node* root);
//int Timingv(Node* root);
int EndView();
typedef enum CERR
{
	CERR_OK,
	CERR_EOF
}CERR;

#endif
