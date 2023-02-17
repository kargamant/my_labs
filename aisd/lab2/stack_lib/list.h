#ifndef LIST
#define LIST

typedef struct Item
{
	char c;
	struct Item* next;
}Item;

typedef struct List
{
	Item* head;
	Item* end;
}List;

List* create_list();
Item* create_item();
void del(List* list);
void push_l(List* list, char c);
void show(List* list);
int enter(List* list);
#endif
