#include "list.h"
#include <stdlib.h>
#include <stdio.h>


List* create_list()
{
	List* list=(List*)malloc(sizeof(List));
	list->head=NULL;
	list->end=NULL;
	return list;
}

//выделение блока памяти для элемента списка
Item* create_item()
{
	Item* item=(Item*)malloc(sizeof(Item));
	item->next=NULL;
	return item; 
}

//очистка списка и его содержимого
void del(List* list)
{
	Item* ptr=list->head;
	while(ptr->next)
	{
		Item* n=ptr->next;
		free(ptr);
		ptr=n;
	}
	free(ptr);
	free(list);
}

//добавление элемента в список на последнее место
void push(List* list, char c)
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
		list->end->next=ptr;
		list->end=ptr;
	}
}

//ввод списка
int enter(List* list)
{
	char c;
	while((c=getchar())!='\n' && c!=EOF) push(list, c);
	if(c==EOF) return 1;
	else return 0;
}

//вывод списка
void show(List* list)
{
	printf("\"");
	Item* ptr=list->head;
	while(ptr)
	{
		printf("%c", ptr->c);
		ptr=ptr->next;
	}
	printf("\"\n");
}


