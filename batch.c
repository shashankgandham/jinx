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

#include "batch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_batch(char *database, batch *xbatch) {
	FILE *fp;
	int n;
	char file[64];
	n = batch_number(database);
	if(n) {
		batch ybatch;
		ybatch = get_batch(database, n-1);
		xbatch -> index = ybatch.index + 1;
	}
	else
		xbatch->index = 0;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_batch");
	fp = fopen(file,"a");
	fprintf(fp,"%d %d %d %s\n",xbatch->index,xbatch->parent,xbatch->strength,xbatch->name);
	fclose(fp);
	return 0;
}

int remove_batch(char *database, int index){
	FILE *fp;
	int n, i;
	char file[64],temp_file[64];
	n = batch_number(database);
	batch xbatch;	
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_batch");
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_batch");
	fp = fopen(temp_file,"w");
	for(i = 0; i < n; i++) {
		if(i != index) {
			xbatch = get_batch(database, i);
			fprintf(fp,"%d %d %d %s\n",xbatch.index,xbatch.parent,xbatch.strength ,xbatch.name);
		}
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}

int edit_batch(char *database, int index, batch *xbatch){
	FILE *fp;
	int n, i;
	char file[64], temp_file[64];
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_batch");
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_batch");
	fp = fopen(temp_file,"a");
	n = batch_number(database);
	batch ybatch;
	for(i = 0; i < n; i++) {
		ybatch = get_batch(database, n-1);
		if(i!=index)
			fprintf(fp,"%d %d %d %s\n",ybatch.index,ybatch.parent,ybatch.strength, ybatch.name);
		else
			fprintf(fp,"%d %d %d %s\n",xbatch->index,xbatch->parent,xbatch->strength, xbatch->name);
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}

batch get_batch(char *database, int index) {
	FILE *fp;
	int n = 0;
	char file[64];
	batch xbatch;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_batch");
	fp = fopen(file,"r");
	while(fscanf(fp,"%d %d %d %[^\n]s",&xbatch.index, &xbatch.parent,&xbatch.strength,xbatch.name) != EOF) {
		if(n == index)
			break;
		n++;
	}
	fclose(fp);
	return xbatch;
}
int batch_number(char *database) {
	FILE *fp;
	int n = 0;
	char file[64];
	batch xbatch;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_batch");
	fp = fopen(file,"r");
	if(fp == NULL) {
		fp = fopen(file,"w");
		fclose(fp);
		return 0;
	}
	while(fscanf(fp,"%d %d %d %[^\n]s",&xbatch.index, &xbatch.parent, &xbatch.strength, xbatch.name) != EOF)
		n++;
	fclose(fp);
	return n;
}

int *find_batch_info(char *database, int index){

}
int sort_batch(char *database , int(*compare)(const void *x ,const void *y)){

	return 0;
}
int start_batch(char *database){
	int n, total;
	while(1) {
		n = batch_menu(database);
		total = batch_number(database) + 2;
		if(n == total - 1)
			break;
		if(total - 2 == n)
			batch_form(database);
	}
	return 0;
}
int batch_menu(char *database){
	int i, c, n, choice;
	batch *xbatch;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	n = batch_number(database);
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);	
	xbatch = (batch *)malloc(sizeof(batch) * (n + 1));
	items = (ITEM **)calloc(n + 3, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		xbatch[i] = get_batch(database,i);
		items[i] = new_item(xbatch[i].name, NULL);
	}
	items[i] = new_item("Add Batch",NULL);
	items[i + 1] = new_item("Back",NULL);
	n+=2;
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
	print_in_middle(win, 1, 0, x, "Batchs" , COLOR_PAIR(1));
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
				break;
			
			default:
				break;
		}
		if(c == 10)
			break;
		wrefresh(win);
	}	
	free(xbatch);
	return choice;
}
int batch_form(char *database){
	refresh();
	batch xbatch;
	echo();
	WINDOW *win;
	int y,x;	
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(6, 40, y/3, x/3);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, 40, "Enter the Name of Batch", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw(" %[^\n]s",xbatch.name);
	clear();
	box(win, 0, 0);
/*	print_in_middle(win, 1, 0, 40, "Enter the parent the batch", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw("%d",&xbatch.parent);*/
	xbatch.parent = -1;
	print_in_middle(win, 1, 0, 40, "Enter the strength of the class", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw("%d",&xbatch.strength);
	add_batch(database, &xbatch);
	refresh();
	endwin();
	clear();
	return 0;
}
