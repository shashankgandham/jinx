/*  This file is part of Jinx originally written by Shashank gandham.
    Timetable Generator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.
    Timetable Generator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Jinx.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "room.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
int add_room(char *database, room *xroom) {
	FILE *fp;
	int n;
	char file[64];
	n = room_number(database);
	if(n) {
		room yroom;
		yroom = get_room(database, n-1);
		xroom -> index = yroom.index + 1;
	}
	else
		xroom->index = 0;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_room");
	fp = fopen(file,"a");
	fprintf(fp,"%d %d %s\n",xroom->index,xroom->capacity,xroom->name);
	fclose(fp);
	return 0;
}
int remove_room(char *database, int index){
	FILE *fp;
	int n, i;
	char file[64],temp_file[64];
	n = room_number(database);
	room xroom;
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_room");
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_room");
	fp = fopen(temp_file,"w");
	for(i = 0; i < n; i++) {
		if(i != index) {
			xroom = get_room(database, i);
			fprintf(fp,"%d %d %s\n",xroom.index,xroom.capacity, xroom.name);
		}
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}
int edit_room(char *database, int index, room *xroom){
	FILE *fp;
	int n, i;
	char file[64], temp_file[64];
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_room");
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_room");
	fp = fopen(temp_file,"a");
	n = room_number(database);
	room yroom;
	for(i = 0; i < n; i++) {
		yroom = get_room(database, n-1);
		if(i!=index)
			fprintf(fp,"%d %d %s\n",yroom.index,yroom.capacity, yroom.name);
		else
			fprintf(fp,"%d %d %s\n",xroom->index,xroom->capacity, xroom->name);
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}
room get_room(char *database, int index) {
	FILE *fp;
	int n = 0;
	char file[64];
	room xroom;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_room");
	fp = fopen(file,"r");
	while(fscanf(fp,"%d %d %[^\n]s",&xroom.index, &xroom.capacity, xroom.name) != EOF) {
		if(n == index)
			break;
		n++;
	}
	fclose(fp);
	return xroom;
}
int room_number(char *database) {
	FILE *fp;
	int n = 0;
	char file[64];
	room xroom;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_room");
	fp = fopen(file,"r");
	if(fp == NULL) {
		fp = fopen(file,"w");
		fclose(fp);
		return 0;
	}
	while(fscanf(fp,"%d %d %[^\n]s",&xroom.index, &xroom.capacity, xroom.name) != EOF)
		n++;
	fclose(fp);
	return n;
}
int sort_room(char *database , int(*compare)(const void *x ,const void *y)){
	return 0;
}
int show_room_info(char *database, int choice) {
	int c;
	room xroom;
	xroom = get_room(database,choice);
	refresh();
	noecho();
	curs_set(0);
	WINDOW *win;
	int y,x;
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(0, 0, 0, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Info", COLOR_PAIR(1));
	mvwhline(win, y/4, x/4, ACS_HLINE, x/2);
	mvwhline(win, y/4 + 2, x/4, ACS_HLINE, x/2);
	mvwhline(win, y/2, x/4, ACS_HLINE, x/2);
	mvwvline(win, y/4 + 1, x/4 , ACS_VLINE, y/4 - 1);
	mvwaddch(win, y/4, x/4 , ACS_ULCORNER);
	mvwaddch(win, y/2, x/4 , ACS_LLCORNER);
	mvwaddch(win, y/4, 3*x/4 , ACS_URCORNER);
	mvwaddch(win, y/2, 3*x/4 , ACS_LRCORNER);
	mvwvline(win, y/4 + 1, 3*x/4, ACS_VLINE, y/4 - 1);
	mvwaddch(win, y/4 + 2, 3*x/4 , ACS_RTEE);
	mvwaddch(win, y/4 + 2, x/4 , ACS_LTEE);
	wrefresh(win);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	move(y/4 + 3,x/3 + 2);
	printw("Name - %s",xroom.name);
	move(y/4 + 5,x/3 + 2);
	printw("Max Capacity - %d",xroom.capacity);
	mvwprintw(win,y - 2, 2,"B:Back\tQ:Quit");
	refresh();
	while((c = wgetch(win))){
		switch(c) {
			case KEY_DOWN:
			case KEY_UP:
				return 0;
			case 'B':
			case 'b':
				curs_set(1);
				return 1;
			case 'Q':
			case 'q':
				curs_set(1);
				return INT_MIN;			
			default:
				break;
		}
		wrefresh(win);
	}
	curs_set(1);
	return 0;
}
int start_room(char *database){
	int choice, n, sub_choice;
	while(1) {
		choice = room_menu(database);
		n = room_number(database);
		if(choice == n + 1)
			break;
		else if(choice == n + 2)
			room_form(database);
		else if(choice == INT_MIN)
			return INT_MIN;
		else if(choice == -1)
			continue;
		else{
			while(1) {
				sub_choice = show_room_info(database, choice);
				if(sub_choice == 1)
					break;
				if(sub_choice == INT_MIN)
					return INT_MIN;
			}
		}
	}
	return 0;
}
int room_menu(char *database){
	int i, c, n, choice = 0;
	room *xroom;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	n = room_number(database);
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	xroom = (room *)malloc(sizeof(room) * (n + 1));
	items = (ITEM **)calloc(n + 3, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		xroom[i] = get_room(database,i);
		items[i] = new_item(xroom[i].name, NULL);
	}
	menu = new_menu((ITEM **)items);
	win = newwin(0, 0, 0, 0);
	int y,x;
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	set_menu_spacing(menu, 0, 2, 0);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, "Rooms" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {
		if(n > 1)
			mvwprintw(win,y - 2, 2,"N:New Room\t\tR:Remove Room\tS:Sort\t\tB:Back\tQ:Quit");
		else
			mvwprintw(win,y - 2, 2,"N:New Room\t\tR:Remove Room\tB:Back\tQ:Quit");
		for(i = 0; i < choice; i++)
			menu_driver(menu, REQ_DOWN_ITEM);
		post_menu(menu);
		wrefresh(win);
		while((c = wgetch(win))){
			switch(c) {
				case KEY_DOWN:
					menu_driver(menu, REQ_DOWN_ITEM);
					if(choice != n -1)
						choice++;
					break;
				case KEY_UP:
					menu_driver(menu, REQ_UP_ITEM);
					if(choice != 0)
						choice--;
					break;
				case 10: /* Enter */
					remove_menu(menu,items,n);
					return choice;
				case 'R':
				case 'r':
					remove_room(database, choice);
					remove_menu(menu,items,n);
					return -1;
				case 'B':
				case 'b':
					remove_menu(menu,items,n);
					return n + 1;
				case 'N':
				case 'n':
					remove_menu(menu,items,n);
					return n + 2;
				case 'Q':
				case 'q':
					remove_menu(menu,items,n);
					return INT_MIN;
				default:
					break;
			}
			wrefresh(win);
		}
	}
	else {
		mvwprintw(win,y - 2, 2,"N:New Room\t\tB:Back\t\tQ:Quit");
		mvwprintw(win,5,3*x/7,"No Rooms found :(\n");
		wrefresh(win);
		curs_set(0);
		while((c = wgetch(win)))
		{
			switch(c) {
				case 'n':
				case 'N':
					remove_menu(menu,items,n);
					curs_set(1);
					return n+2;
				case 'b':
				case 'B':
					remove_menu(menu,items,n);
					curs_set(1);
					return n+1;
				case 'Q':
				case 'q':
					remove_menu(menu,items,n);
					curs_set(1);
					return INT_MIN;
				default:
					break;
			}
			wrefresh(win);
		}
	}
	free(xroom);
	return -1;
}
int room_form(char *database){
		refresh();
	room xroom;
	echo();
	WINDOW *win;
	int y,x;
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(0, 0, 0, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Enter the Name of Room", COLOR_PAIR(1));
	print_form(win, y,x);	
	scanstr(win,xroom.name,x/3 - 1);
	clear();
	refresh();
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Enter the capacity of the room", COLOR_PAIR(1));
	print_form(win, y,x);	
	scanw("%d",&xroom.capacity);
	add_room(database, &xroom);
	refresh();
	endwin();
	clear();
	return 0;
}
