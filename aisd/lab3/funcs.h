#ifndef FUNCS_H
#define FUNCS_H

char* enter();
char* enterf(FILE* fd);
int GetIntf(FILE* fd);
Table* TableWrite(Table* t, char* fn);
int menue();
int arrow(int cur, int pos);
int console(int input, Table* t);
Table* fimport();
KeySpace* keysearch(Table* t);
Node* versionsearch(Table* t);
typedef enum CERR
{
	CERR_OK,
	CERR_EOF
}CERR;

#endif
