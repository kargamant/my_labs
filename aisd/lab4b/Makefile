all:
	gcc -o main main.c BinTree.c funcs.c menue.c 
tree:
	gcc -c -g -O0 Btree.c funcs.c timer.c -fpic 
	gcc -g -O0 Btree.o funcs.o timer.o -shared -o libbtree.so
	gcc -c -O0 menue.c -fpic
	gcc menue.o -shared -o libmenue.so
	gcc -c -g -O0 main.c funcs.c Btree.c menue.c timer.c
	gcc -o -g -O0 main main.o -L. -lbtree -lmenue
	mv Btree.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv menue.o binaries
	mv timer.o binaries
