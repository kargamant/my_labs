txt-arrow:
	gcc -c TableLib/Table.c funcs.c -fpic -lncurses
	gcc Table.o funcs.o -shared -o libtable.so
	gcc -c main.c funcs.c TableLib/Table.c arrow_menue.c -lncurses
	gcc -o main main.o arrow_menue.o -L. -ltable -lncurses
	mv Table.o binaries
	mv funcs.o binaries
	mv libtable.so binaries
	mv main.o binaries
	mv arrow_menue.o binaries
txt:
	gcc -c TableLib/Table.c funcs.c -fpic
	gcc Table.o funcs.o -shared -o libtable.so
	gcc -c main.c funcs.c TableLib/Table.c menue.c
	gcc -o main main.o menue.o -L. -ltable
	mv Table.o binaries
	mv funcs.o binaries
	mv libtable.so binaries
	mv main.o binaries
	mv menue.o binaries
test:
	gcc -o main main.c funcs.c TableLib/Table.c -lncurses
