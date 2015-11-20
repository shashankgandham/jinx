/*  This file is part of Jinx originally written by Shashank Gandham.

    Jinx is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Jinx is distributed in the hope that it will be useful,
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

int add_room(char *database, room *xroom) {
	FILE *fp;
	int n;
	char file[16];
	n = room_number(database);
	if(n) {
		room *yroom;
		yroom = (room *)malloc(sizeof(room));
		*yroom = get_room(database, n-1);
		xroom -> index = yroom->index + 1;
		free(yroom);
	}
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
	int n, i , k = 0;
	char file[16];
	n = room_number(database);
	room *xroom;
	xroom = (room *)malloc(sizeof(room) * n);
	for(i = 0; i < n; i++) {
		if(i != index) {
			xroom[k] = get_room(database, n-1);
			k++;
		}
	}
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_room");
	fp = fopen(file,"a");
	for(i = 0; i < n - 1; i++)
		fprintf(fp,"%d %d %s\n",xroom[i].index,xroom[i].capacity, xroom[i].name);
	fclose(fp);
	free(xroom);
	return 0;
}
int edit_room(char *database, int index, char *name){
	FILE *fp;
	int n, i , k = 0;
	char file[16];
	n = room_number(database);
	room *xroom;
	xroom = (room *)malloc(sizeof(room) * n);
	for(i = 0; i < n; i++) {
		xroom[k] = get_room(database, n-1);
		k++;
	}
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_room");
	strcpy(xroom[index].name, name);
	fp = fopen(file,"a");
	for(i = 0; i < n; i++)
		fprintf(fp,"%d %d %s\n",xroom[i].index,xroom[i].capacity, xroom[i].name);
	fclose(fp);
	free(xroom);
	return 0;
}
room get_room(char *database, int index) {
	FILE *fp;
	int n = 0;
	char file[16];
	room xroom;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_room");
	fp = fopen(file,"r");
	while(fscanf(fp,"%d %d %[^\n]s",&xroom.index,&xroom.capacity, xroom.name) != EOF) {
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
	char file[16];
	room xroom;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_room");
	fp = fopen(file,"r");
	while(fscanf(fp,"%d %d %[^\n]s",&xroom.index,&xroom.capacity, xroom.name) != EOF)
		n++;
	fclose(fp);
	return n;
}
alloc *find_info(char *database, int index){

}
int sort_room(char *database , int(*compare)(const void *x ,const void *y)){

}
int start_room(char *database){
	int n, total;
	while(1) {
		n = room_menu(database);
		total = room_number(database) + 2;
		if(n == total - 1)
			break;
	}
	return 0;
}
int room_menu(char *database){
	char room_choices[128][128];
	int i, c, n, choice;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	n = room_number(database);
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);	
	items = (ITEM **)calloc(n + 1, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		items[i] = new_item(room_choices[i], NULL);
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
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, "Teachers" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	refresh();
	post_menu(menu);
	wrefresh(win);
	choice = 0;
	while((c = wgetch(win)))
	{       switch(c)
		{	case KEY_DOWN:
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
			
			default:
				break;
		}
		wrefresh(win);
	}	
	return -1;

}
int room_form(char *database){

}
