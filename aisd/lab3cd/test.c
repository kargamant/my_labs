#include <stdio.h>
#include <stdlib.h>
#include "HashLib/Hash.h"
#include "HashLib/HashTable.h"

int main()
{
	Table* ht=create();
	New(5, ht);
	add(ht, 12, "hello\0");
	add(ht, 12, "it's me\0");
	add(ht, 202, "wow\0");
	add(ht, 1024, "monkey\0");
	add(ht, 1, "hog rider\0");
	output(ht);
	//DelByKey(ht, 12);
	printf("----\n");
	Table* find=SearchByVersion(ht, 12, 2);
	//Table* find=SearchByKey(ht, 12);
	output(find);
	printf("----\n");
	erased(find);
	free(find);
	printf("\n");
	output(ht);
	add(ht, 941, "prekol\0");
	output(ht);
	erased(ht);
	free(ht);
	return 0;
}
