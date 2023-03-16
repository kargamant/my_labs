#ifndef FUNCS_H
#define FUNCS_H

char* enter();
char* enterf(FILE* fd);
int GetIntf(FILE* fd);
void TableWrite(Table* t, char* fn);
void menue();
int arrow(int cur, int pos);

#endif
