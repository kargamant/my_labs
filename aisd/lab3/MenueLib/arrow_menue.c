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
	int key=0, pos=1;
	system("clear");	
	
	initscr();
	while(key!=10)
	{
		clear();
		for(int i=1; i<n+1; i++)
		{
			arrow(i, pos);
			printw("%s", options[i-1]);
			refresh();
		}
		arrow(n+1, pos);
		printw("%s", "quit");
		refresh();
		key=getch();
		if(key==115) 
		{
			pos=(pos+1)%10;
			if(!pos) pos=1;
		}
		else if(key==119) 
		{
			pos=(10+(pos-1)%10)%10;
			if(!pos) pos=9;
		}
	}
	endwin();
	return pos;
}


