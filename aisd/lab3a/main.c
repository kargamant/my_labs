#include <stdio.h>
#include <stdlib.h>
#include "TableLib/Table.h"
#include "funcs.h"
#include "MenueLib/menue.h"

int main()
{
	//first initialization of table
	//Table* t=(Table*)calloc(3, sizeof(Table));
	Table* t=create();

	//options for menue
	const char* options[]={
		"create table from console.\n",
		"import table from file.\n",
		"output table.\n",
		"search by key.\n",
		"search by version\n",
		"add\n",
		"delete by key\n",
		"delete by version\n",
		"save changes\n",
		"show raw data and info from files\n",
	};
	
	system("clear");
	//main cycle
	do
	{
		int p=menue(options, 10);
		if(p==11) break;
		int res=console(p, t);
		if(res==CERR_EOF) break;
		system("clear");
	}while(1);
	//To be done later
	//console(8, t);
	erased(t);
	//free(t->fi);
	//free(t->fd);
	free(t);
	t=NULL;
	printf("Program stopped.\n");
	return 0;
}
