#include <stdlib.h>
#include "TableLib/Table.h"
#include "funcs.h"
//interactive menue
int menue()
{
	const char* options[]={
		"import table from file.\n",
		"output table.\n",
		"search by key.\n",
		"search by version\n",
		"add\n",
		"delete by key\n",
		"delete by version\n",
		"save changes\n",
	};
	for(int i=0; i<15; i++) printf("-");
	printf("\n");
	for(int i=1; i<9; i++)
	{
		printf("%d. %s", i, options[i-1]);
	}
	printf("\nEnter EOF to quit.\n");
	for(int i=0; i<15; i++) printf("-");
	printf("\n");

	printf("Choose option: \n");
	int pos=9;
	do
	{
		int in=getInt(&pos);
		if(in) return 9;
		else if(pos<1 || pos>8)
		{
			printf("Incorrect number of option. Try again.\n");
			continue;
		}
		else break;
	}
	while(pos<1 || pos>8);
	return pos;
}
