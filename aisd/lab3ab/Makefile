FLAG = -g
txt-arrow:
	gcc -c TableLib/TableTxt.c funcs.c -fpic -lncurses 
	gcc TableTxt.o funcs.o -shared -o libtable.so
	gcc -c MenueLib/arrow_menue.c -fpic -lncurses
	gcc arrow_menue.o -shared -o libmenue.so
	gcc -c main.c funcs.c TableLib/TableTxt.c MenueLib/arrow_menue.c -lncurses
	gcc -o main main.o -L. -ltable -lmenue -lncurses
	mv TableTxt.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv arrow_menue.o binaries
txt:
	gcc -c TableLib/TableTxt.c funcs.c -fpic
	gcc TableTxt.o funcs.o -shared -o libtable.so
	gcc -c MenueLib/menue.c -fpic
	gcc menue.o -shared -o libmenue.so
	gcc -c main.c funcs.c TableLib/TableTxt.c MenueLib/menue.c
	gcc -o main main.o -L. -ltable -lmenue
	mv TableTxt.o binaries
	mv funcs.o binaries
	mv main.o binaries
	mv menue.o binaries
bin:
	gcc -c TableLib/TableBin.c funcsf.c -fpic 
	gcc TableBin.o funcsf.o -shared -o libtable.so
	gcc -c MenueLib/menue.c -fpic
	gcc menue.o -shared -o libmenue.so
	gcc -c main.c funcsf.c TableLib/TableBin.c MenueLib/menue.c
	gcc -o main main.o -L. -ltable -lmenue
	mv TableBin.o binaries
	mv funcsf.o binaries
	mv main.o binaries
	mv menue.o binaries
bin-arrow:
	gcc -c TableLib/TableBin.c funcsf.c -fpic -lncurses
	gcc TableBin.o funcsf.o -shared -o libtable.so
	gcc -c MenueLib/arrow_menue.c -fpic -lncurses
	gcc arrow_menue.o -shared -o libmenue.so
	gcc -c main.c funcsf.c TableLib/TableBin.c MenueLib/arrow_menue.c -lncurses
	gcc -o main main.o -L. -ltable -lmenue -lncurses
	mv TableBin.o binaries
	mv funcsf.o binaries
	mv main.o binaries
	mv arrow_menue.o binaries

test:
	gcc -o main main.c funcs.c TableLib/Table.c -lncurses
