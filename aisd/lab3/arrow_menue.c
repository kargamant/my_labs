#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>

//displaying arrow in a specific position
int arrow(int cur, int pos)
{
	if(cur==pos)
	{
		printw(">>>");
	}
}

//interactive arrow menue
int menue()
{
	int key=0, pos=1;
	system("clear");
	const char* options[]={
		"import table from file.\n",
		"output table.\n",
		"search by key.\n",
		"search by version\n",
		"add\n",
		"delete by key\n",
		"delete by version\n",
		"save changes\n",
		"quit\n"
	};
	
	initscr();
	while(key!=10)
	{
		clear();
		for(int i=1; i<10; i++)
		{
			arrow(i, pos);
			printw("%s", options[i-1]);
		}
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


