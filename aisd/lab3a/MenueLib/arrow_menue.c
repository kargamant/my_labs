#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>

//displaying arrow in a specific position
int arrow(int cur, int pos)
{
	if(cur==pos)
	{
		printw(">>>");
		refresh();
	}
}

//interactive arrow menue
int menue(const char* options[], int n)
{
	int key=0, pos=0;
	system("clear");
	
	initscr();
	while(key!=10)
	{
		clear();
		for(int i=0; i<n; i++)
		{
			arrow(i, pos);
			printw("%s", options[i-1]);
			refresh();
		}
		arrow(n, pos);
		printw("%s", "quit");
		refresh();
		key=getch();
		if(key==115) 
		{
			pos=(pos+1)%10;
			if(!pos) pos=0;
		}
		else if(key==119) 
		{
			pos=(10+(pos-1)%10)%10;
			if(!pos) pos=10;
		}
	}
	endwin();
	return pos;
}


