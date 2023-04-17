#include <stdio.h>
#include <stdlib.h>
#include "BinTree.h"
#include "funcs.h"
#include "menue.h"

int main()
{
	Node* root=(Node*)calloc(1, sizeof(Node));	
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
	};
	
	system("clear");
	//main cycle
	do
	{
		int p=menue(options, 8);
		if(p==9) break;
		int res=console(p, root);
		if(res==CERR_EOF) break;
		system("clear");
	}while(1);
	erased(root);
	root=NULL;
	printf("Program stopped.\n");
	return 0;
}
