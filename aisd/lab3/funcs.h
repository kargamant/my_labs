#ifndef FUNCS_H
#define FUNCS_H

char* enter();
char* enterf(FILE* fd);
int GetIntf(FILE* fd);
Table* TableWrite(Table* t, char* fn);
int menue();
int arrow(int cur, int pos);
int console(int p, Table* t);
Table* fimport();
KeySpace* keysearch(Table* t);
Node* versionsearch(Table* t);

//view functions
int Inputv(Table* t);
int Outv(Table* t);
int Searchkv(Table* t);
int Searchvv(Table* t);
int Addv(Table* t);
int Delkv(Table* t);
int Delvv(Table* t);
int Savev(Table* t);
int EndView();
typedef enum CERR
{
	CERR_OK,
	CERR_EOF
}CERR;

#endif
