#include <stdio.h>
#include <stdlib.h>
#include "HashLib/HashBin.h"
#include "funcsbin.h"
#include "menue.h"

int main()
{
	//first initialization of table
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
		"show key trajectory\n",
		"save changes\n",
		"iterate key\n",
	};
	
	system("clear");
	//main cycle
	do
	{
		//system("ls");
		int p=menue(options, 11);
		if(p==12) break;
		int res=console(p, t);
		if(res==CERR_EOF) break;
		system("clear");
	}while(1);
	console(9, t);
	if(t->fd) fclose(t->fd);
	erased(t);
	free(t);
	t=NULL;
	printf("Program stopped.\n");

	return 0;
}
