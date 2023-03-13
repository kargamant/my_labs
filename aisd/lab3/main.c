#include <stdio.h>
#include <stdlib.h>
#include "TableLib/Table.h"
#include "funcs.h"

int main()
{
	do
	{
		printf("Enter FileName with table: ");
		const char* FileName=enter();
		if(!FileName) break;
		FILE *fd=fopen(FileName, "r");
		if(!fd)
		{
			printf("File does not exist or wrong FileName. Try again.\n");
			continue;
		}
		Table* t=input(fd);	
		fclose(fd);
		//printf("Data: %d", msize);
	}while(1);
	printf("Program stopped.\n");
	return 0;
}
