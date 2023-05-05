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
		//"Generate random tree\n",
		//"Full timing of the tree\n",
	};
	
//	tr->root->keys[0]=1;
//	tr->root->keys[1]=5;
//	tr->root->keys[2]=10;
//	tr->root->n+=3;
//	tr->root->child[1]->n+=2;
//	tr->root->child[2]->n=1;
//	tr->root->child[1]->keys[0]=3;
//	tr->root->child[1]->keys[1]=4;
//	tr->root->child[2]->keys[0]=7;
//	show(tr, 0);
//	return 0;

	system("clear");
	//main cycle
	do
	{
		printf("root: \n");
		printf("n: %d\n", tr->root->n);
		int p=menue(options, 8);
		if(p==9) break;
		int res=console(p, tr);
		if(res==CERR_EOF) break;
		system("clear");
	}while(1);
	printf("Program stopped.\n");
	return 0;
}
