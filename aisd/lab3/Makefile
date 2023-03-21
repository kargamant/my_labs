txt-arrow:
	gcc -c TableLib/Table.c funcs.c -fpic -lncurses
	gcc Table.o funcs.o -shared -o libtable.so
	gcc -c MenueLib/arrow_menue.c -fpic -lncurses
	gcc arrow_menue.o -shared -o libmenue.so
	gcc -c main.c funcs.c TableLib/Table.c MenueLib/arrow_menue.c -lncurses
	gcc -o main main.o -L. -ltable -lmenue -lncurses
	mv Table.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv arrow_menue.o binaries
txt:
	gcc -c TableLib/Table.c funcs.c -fpic
	gcc Table.o funcs.o -shared -o libtable.so
	gcc -c MenueLib/menue.c -fpic
	gcc menue.o -shared -o libmenue.so
	gcc -c main.c funcs.c TableLib/Table.c MenueLib/menue.c
	gcc -o main main.o -L. -ltable -lmenue
	mv Table.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv menue.o binaries
test:
	gcc -o main main.c funcs.c TableLib/Table.c -lncurses
