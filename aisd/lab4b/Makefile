all:
	gcc -o main main.c BinTree.c funcs.c menue.c 
tree:
	gcc -c Btree.c funcs.c -fpic 
	gcc Btree.o funcs.o -shared -o libbtree.so
	gcc -c menue.c -fpic
	gcc menue.o -shared -o libmenue.so
	gcc -c main.c funcs.c Btree.c menue.c
	gcc -o main main.o -L. -lbtree -lmenue
	mv Btree.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv menue.o binaries
