#include <stdio.h>
#include <stdlib.h>
#include "TableLib/Table.h"
#include "funcs.h"

int main()
{
	Table* t=fimport();
	if(!t) goto endp;
	do
	{
		int p=menue();
		if(p==9) break;
		int res=console(p, t);
		if(res==CERR_EOF) break;
	}while(1);
	erased(t);
	endp: printf("Program stopped.\n");
	//printf("%d", p);
	/*
	do
	{
		printf("Enter FileName with table: ");
		char* FileName=enter();
		if(!FileName) break;
		FILE *fd=fopen(FileName, "r");
		if(!fd)
		{
			printf("File does not exist or wrong FileName. Try again.\n");
			continue;
		}
		Table* t=input(fd);	
		if(!t)
		{
			printf("Error. Got wrong data while parsing. Try again.\n");
			continue;
		}
		fclose(fd);
		output(t);
		DelByKey(t, 31);
		//DelByKey(t, 73);
		output(t);
		TableWrite(t, FileName);
		erase(t);
		free(FileName);
		FileName=NULL;
		//printf("Data: %d", msize);
	}while(1);
	printf("Program stopped.\n");*/
	return 0;
}
