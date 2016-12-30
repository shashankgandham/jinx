/*
	MIT LICENSE Copyright (c) 2016 Shashank Gandham
*/

#include "library.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void remove_menu(MENU *menu, ITEM **items, int n) {
	int i;
	unpost_menu(menu);
	free_menu(menu);
	for(i = 0; i < n; ++i)
		free_item(items[i]);
	endwin();
	clear();
	refresh();
}
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color) {
	int length, x, y;
	float temp;
	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;
	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}
void print_form_str(WINDOW *win, int y, int x) {
	mvwhline(win, y/4, x/3, ACS_HLINE, x/3 + 1);
	mvwhline(win, y/4 + 2, x/3, ACS_HLINE, x/3 + 1);
	mvwhline(win, y/4 + 5, x/3, ACS_HLINE, x/3 + 1);

	mvwvline(win, y/4 + 1, x/3 , ACS_VLINE, 4);
	mvwvline(win, y/4 + 1, 2*x/3, ACS_VLINE, 4);

	mvwaddch(win, y/4, x/3 , ACS_ULCORNER);
	mvwaddch(win, y/4, 2*x/3 , ACS_URCORNER);
	
	mvwaddch(win, y/4 + 5, x/3 , ACS_LLCORNER);
	mvwaddch(win, y/4 + 5, 2*x/3 , ACS_LRCORNER);
	
	mvwaddch(win, y/4 + 2, 2*x/3 , ACS_RTEE);
	mvwaddch(win, y/4 + 2, x/3 , ACS_LTEE);
	move(y/4 + 3,x/3 + 1);
	wrefresh(win);
}
void print_form_int(WINDOW *win, int y, int x) {
	mvwhline(win, y/4, x/3, ACS_HLINE, x/3 + 1);
	mvwhline(win, y/4 + 2, x/3, ACS_HLINE, x/3 + 1);
	mvwhline(win, y/4 + 5, x/3, ACS_HLINE, x/3 + 1);

	mvwvline(win, y/4 + 1, x/3 , ACS_VLINE, 4);
	mvwvline(win, y/4 + 1, 2*x/3, ACS_VLINE, 4);

	mvwaddch(win, y/4, x/3 , ACS_ULCORNER);
	mvwaddch(win, y/4, 2*x/3 , ACS_URCORNER);
	
	mvwaddch(win, y/4 + 5, x/3 , ACS_LLCORNER);
	mvwaddch(win, y/4 + 5, 2*x/3 , ACS_LRCORNER);
	
	mvwaddch(win, y/4 + 2, 2*x/3 , ACS_RTEE);
	mvwaddch(win, y/4 + 2, x/3 , ACS_LTEE);
	move(y/4 + 3,x/2);
	wrefresh(win);
}
int scanstr(WINDOW *win, char *str, int len) {
	int i = 0, c, x, y;
	getyx(win,y,x);
	while((c = getch())) {
		if(c == 10) {
			str[i] = 0;
			break;
		}
		else if((c == KEY_DC || c == KEY_BACKSPACE) && i){
			move(y + 1,x + i - 1);
			delch();
			refresh();
			i--;
		}
		else if(isalnum(c) || isspace(c) || ispunct(c)) {
			if(i == len - 1)
				continue;
			str[i] = c;
			printw("%c",str[i]);
			refresh();
			i++;
		}
	}
	return 0;
}
int scanint(WINDOW *win) {
	char str[4];
	int len = 3;
	int i = 0, c, x, y;
	getyx(win,y,x);
	while((c = getch())) {
		if(c == 10) {
			str[i] = 0;
			break;
		}
		else if((c == KEY_DC || c == KEY_BACKSPACE) && i){
			move(y + 1,x + i - 1);
			delch();
			refresh();
			i--;
		}
		else if(isalnum(c) || isspace(c) || ispunct(c)) {
			if(i == len - 1)
				continue;
			str[i] = c;
			printw("%c",str[i]);
			refresh();
			i++;
		}
	}
	x = 0;
	if(i == 1) {
		x = str[0] - '0';
	}
	else if(i == 2)
		x = (str[0] - '0')*10 + (str[1] - '0');
	return x;
}
