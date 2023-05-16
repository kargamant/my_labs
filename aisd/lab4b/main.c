#include <stdio.h>
#include <stdlib.h>
#include "Btree.h"
#include "funcs.h"
#include "menue.h"

int main()
{
	//Node* root=(Node*)calloc(1, sizeof(Node));	
	Btree* tr=InitBtree(2);
	//options for menue
	const char* options[]={
		"Import tree from text file\n",
		"Traversing of tree\n",
		"Add node to tree\n",
		"Delete node from tree\n",
		"Search node by key\n",
		"Find Maximum\n",
		"Find Minimum\n",
		"Show Tree as a tree\n",
		"Full timing of the tree\n",
		"Generate random tree\n",
	};
	
	system("clear");
	//main cycle
	do
	{
		//printf("%s\n", options[9]);
		printf("root: \n");
		printf("n: %d\n", tr->root->n);
		int p=menue(options, 10);
		if(p==11) break;
		int res=console(p, tr);
		if(res==CERR_EOF) break;
		system("clear");
	}while(1);
	printf("Program stopped.\n");
	erase(tr);
	//free(tr->root);
	free(tr);
	return 0;
}
