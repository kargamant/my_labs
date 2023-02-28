#include "list.h"
#include <stdlib.h>
#include <stdio.h>

//From lab6 in previous term

//allocating new block of memory for list
List* create_list()
{
	List* list=(List*)calloc(1, sizeof(List));
	list->head=NULL;
	list->end=NULL;
	return list;
}

//allocating new block of memory for single item
Item* create_item()
{
	Item* item=(Item*)malloc(sizeof(Item));
	item->next=NULL;
	return item; 
}

//erasing list and it's items
void del(List* list)
{
	Item* ptr=list->head;
	if(ptr!=NULL)
	{
		while(ptr->next)
		{
			Item* n=ptr->next;
			free(ptr);
			ptr=n;
		}
		free(ptr);
	}
	free(list);
}

//pushing element to end of the list
void push_l(List* list, char* c)
{
	Item* ptr=create_item();	
	ptr->c=c;
	if(list->head==NULL)
	{
		list->head=ptr;
		list->end=ptr;
	}
	else
	{
		ptr->next=list->head;
		list->head=ptr;
	}
}

/*

//Input of a list
int enter(List* list)
{
	char* c;
	while((*c=getchar())!='\n' && *c!=EOF) push_l(list, c);
	if(*c==EOF) return 1;
	else return 0;
}
*/

//Output of a list
void show(List* list)
{
	printf("\"");
	Item* ptr=list->head;
	while(ptr)
	{
		printf("%c", *(ptr->c));
		ptr=ptr->next;
	}
	printf("\"\n");
}
