#include <stdio.h>
#include <stdlib.h>
#include "TableLib/Table.h"
#include "funcs.h"
#include "menue.h"

int main()
{
	//first initialization of table
	Table* t=(Table*)calloc(3, sizeof(Table));
	console(1, t);
	if(!t) goto endp;
	
	system("clear");
	//main cycle
	do
	{
		int p=menue();
		if(p==9) break;
		int res=console(p, t);
		if(res==CERR_EOF) break;
		system("clear");
	}while(1);
	erased(t);
	free(t);
	t=NULL;
	endp: printf("Program stopped.\n");
	return 0;
}
