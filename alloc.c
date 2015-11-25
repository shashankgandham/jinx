/*  This file is part of Jinx originally written by Shashank Gandham.

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

#include "alloc.h"
#include "teacher.h"
#include "batch.h"
#include "subject.h"
#include "room.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>

int add_alloc(char *database, alloc *xalloc) {
	FILE *fp;
	int n;
	char file[64];
	n = alloc_number(database);
	if(n) {
		alloc yalloc;
		yalloc = get_alloc(database, n-1);
		xalloc -> index = yalloc.index + 1;
	}
	else
		xalloc->index = 0;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_alloc");
	fp = fopen(file,"a");
	fprintf(fp,"%d %d %d %d %d\n",xalloc->index,xalloc->batch,xalloc->subject,xalloc->teacher,xalloc->slot);
	fclose(fp);
	return 0;
}
int remove_alloc(char *database, int index){
	FILE *fp;
	int n, i;
	char file[64],temp_file[64];
	n = alloc_number(database);
	alloc xalloc;
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_alloc");
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_alloc");
	fp = fopen(temp_file,"w");
	for(i = 0; i < n; i++) {
		if(i != index) {
			xalloc = get_alloc(database, i);
			fprintf(fp,"%d %d %d %d %d\n",xalloc.index,xalloc.batch, xalloc.subject,xalloc.teacher,xalloc.slot);
		}
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}
int edit_alloc(char *database, int index, alloc *xalloc){
	FILE *fp;
	int n, i;
	char file[64], temp_file[64];
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_alloc");
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_alloc");
	fp = fopen(temp_file,"a");
	n = alloc_number(database);
	alloc yalloc;
	for(i = 0; i < n; i++) {
		yalloc = get_alloc(database, n-1);
		if(i!=index)
			fprintf(fp,"%d %d %d %d %d\n",yalloc.index,yalloc.batch, yalloc.subject,yalloc.teacher,yalloc.slot);
		else
			fprintf(fp,"%d %d %d %d %d\n",xalloc->index,xalloc->batch, xalloc->subject,xalloc->teacher,xalloc->slot);
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}
alloc get_alloc(char *database, int index) {
	FILE *fp;
	int n = 0;
	char file[64];
	alloc xalloc;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_alloc");
	fp = fopen(file,"r");
	while(fscanf(fp,"%d %d %d %d %d",&xalloc.index, &xalloc.batch, &xalloc.subject, &xalloc.teacher,&xalloc.slot) != EOF) {
		if(n == index)
			break;
		n++;
	}
	fclose(fp);
	return xalloc;
}
int alloc_number(char *database) {
	FILE *fp;
	int n = 0;
	char file[64];
	alloc xalloc;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_alloc");
	fp = fopen(file,"r");
	if(fp == NULL) {
		fp = fopen(file,"w");
		fclose(fp);
		return 0;
	}
	while(fscanf(fp,"%d %d %d %d %d",&xalloc.index, &xalloc.batch, &xalloc.subject, &xalloc.teacher, &xalloc.slot) != EOF)
		n++;
	fclose(fp);
	return n;
}
int *find_alloc_info(char *database, int index){
	return 0;
}
int sort_alloc(char *database , int(*compare)(const void *x, const void *y)){ 	
	return 0;
}
int start_alloc(char *database){
	int choice, n;
	while(1) {
		choice = alloc_menu(database);
		n = alloc_number(database);

		if(choice == n + 1)
			break;

		else if(choice == n + 2)
			alloc_form(database);

		else if(choice == INT_MIN)
			return INT_MIN;

		else if(choice == -1)
			continue;
	}
	return 0;
}

int alloc_menu(char *database){
	int i, c, n, choice = 0;
	alloc xalloc;
	char **choices, **description;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	n = alloc_number(database);
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	choices = (char **)malloc(sizeof(char *) * n + 1);
	description = (char **)malloc(sizeof(char *) * n + 1);
	items = (ITEM **)calloc(n + 3, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		xalloc = get_alloc(database,i);
		strcpy(choices, get_batch(database, get_array_index(database, 0, xalloc.batch)).name);
		items[i] = new_item(choices[i], NULL);
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
	print_in_middle(win, 1, 0, x, "Allocations" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {

		if(n > 1)
			mvwprintw(win,y - 2, 2,"N:New Allocation\t\tR:Remove Allocation\tS:Sort\t\tB:Back\tQ:Quit");
		else
			mvwprintw(win,y - 2, 2,"N:New Allocation\t\tR:Remove Allocation\tB:Back\tQ:Quit");
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
					remove_alloc(database, choice);
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
		mvwprintw(win,y - 2, 2,"N:New Allocation\t\tB:Back\t\tQ:Quit");
		mvwprintw(win,5,3*x/7,"No Allocations found :(\n");
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
	return -1;
}
int alloc_teacher(char *database){
	int i, c, n, choice = 0;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	win = newwin(0,0,0,0);
	teacher *xteacher;
	n = teacher_number(database);
	xteacher = (teacher *)malloc(sizeof(teacher) * (n + 1));
	items = (ITEM **)calloc(n + 3, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		xteacher[i] = get_teacher(database,i);
		items[i] = new_item(xteacher[i].name, NULL);
	}
	menu = new_menu((ITEM **)items);
	int y,x;
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	set_menu_spacing(menu, 0, 2, 0);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, "Teachers" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {
		mvwprintw(win,y - 2, 2,"Enter:Select\tB:Back\tQ:Quit");
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

				case 'B':
				case 'b':
					remove_menu(menu,items,n);
					return n + 1;
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
	free(xteacher);
	endwin();
	clear();
	refresh();
	return 0;
}
int alloc_subject(char *database){
	int i, c, n, choice = 0;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	win = newwin(0,0,0,0);
	subject *xsubject;
	n = subject_number(database);
	xsubject = (subject *)malloc(sizeof(subject) * (n + 1));
	items = (ITEM **)calloc(n + 3, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		xsubject[i] = get_subject(database,i);
		items[i] = new_item(xsubject[i].name, NULL);
	}
	menu = new_menu((ITEM **)items);
		int y,x;
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	set_menu_spacing(menu, 0, 2, 0);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, "Teachers" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {
		mvwprintw(win,y - 2, 2,"Enter:Select\tB:Back\tQ:Quit");
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

				case 'B':
				case 'b':
					remove_menu(menu,items,n);
					return n + 1;
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
	free(xsubject);
	endwin();
	clear();
	refresh();
	return 0;
}
int alloc_batch(char *database){
	int i, c, n, choice = 0;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	win = newwin(0,0,0,0);
	batch *xbatch;
	n = batch_number(database);
	xbatch = (batch *)malloc(sizeof(batch) * (n + 1));
	items = (ITEM **)calloc(n + 3, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		xbatch[i] = get_batch(database,i);
		items[i] = new_item(xbatch[i].name, NULL);
	}
	menu = new_menu((ITEM **)items);
		int y,x;
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	set_menu_spacing(menu, 0, 2, 0);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, "Teachers" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {
		mvwprintw(win,y - 2, 2,"Enter:Select\tB:Back\tQ:Quit");
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

				case 'B':
				case 'b':
					remove_menu(menu,items,n);
					return n + 1;
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
	free(xbatch);
	endwin();
	clear();
	refresh();
	return 0;
}
int get_index(char *database, int datum, int index) {
	if(datum == 2) {
		if(index > teacher_number(database))
		       return -1;	
		return get_teacher(database, index).index;
	}
	if(datum == 0) {
		if(index > batch_number(database))
		       return -1;	
		return get_batch(database, index).index;
	}	
	if(datum == 1) {
		if(index > subject_number(database))
		       return -1;	
		return get_subject(database, index).index;
	}	
	if(datum == 3) {
		if(index > room_number(database))
		       return -1;	
		return get_room(database, index).index;
	}
}
int get_array_index(char *database, int datum, int index) {
	int i, n;	
	if(datum == 2) {
		n = teacher_number(database);
		for(i = 0; i < n; i++)
		if(get_teacher(database, index).index == index)
			return i;
	}
	if(datum == 0) {
		n = batch_number(database);
		for(i = 0; i < n; i++)
		if(get_batch(database, index).index == index)
			return i;	
	}	
	if(datum == 1) {
		n = subject_number(database);
		for(i = 0; i < n; i++)
		if(get_subject(database, index).index == index)
			return i;	
	}	
	if(datum == 3) {
		n = teacher_number(database);
		for(i = 0; i < n; i++)
		if(get_room(database, index).index == index)
			return i;	
	}	
}

int alloc_form(char *database) {
	alloc xalloc;
	xalloc.teacher = get_index(database, 2, alloc_teacher(database));
	xalloc.subject = get_index(database, 1, alloc_subject(database));
	xalloc.batch = get_index(database, 0, alloc_batch(database));
	add_alloc(database, &xalloc);
	return 0;
}
