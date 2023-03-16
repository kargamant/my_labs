txt:
	gcc -c TableLib/Table.c funcs.c -fpic -lncurses
	gcc Table.o funcs.o -shared -o libtable.so
	gcc -c main.c funcs.c TableLib/Table.c -lncurses
	gcc -o main main.o -L. -ltable -lncurses
	mv Table.o binaries
	mv funcs.o binaries
	mv libtable.so binaries
	mv main.o binaries
test:
	gcc -o main main.c funcs.c TableLib/Table.c -lncurses
