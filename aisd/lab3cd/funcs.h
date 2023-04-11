#ifndef FUNCS_H
#define FUNCS_H
#include <stdio.h>
#include "HashLib/HashTable.h"

char* enter();
char* enterf(FILE* fd);
int GetIntf(FILE* fd);
int getInt(int* n);
int fcheck(char* fnd);
//int TableWrite(Table* t, char* fn);
int menue();
//int arrow(int cur, int pos);
int console(int p, Table* t);
//Table* fimport();
//KeySpace* keysearch(Table* t);
//Node* versionsearch(Table* t);

//view functions
int KeyTr(Table* t);
int Newv(Table* t);
//int Inputv(Table* t);
int Outv(Table* t);
int Searchkv(Table* t);
int Searchvv(Table* t);
int Addv(Table* t);
int Delkv(Table* t);
int Delvv(Table* t);
//int Savev(Table* t);
//int Rawv(Table* t);
int EndView();
typedef enum CERR
{
	CERR_OK,
	CERR_EOF
}CERR;

#endif
