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

#include "teacher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>

int add_teacher(char *database, teacher *xteacher) {
	FILE *fp;
	int n;
	char file[64];
	n = teacher_number(database);
	if(n) {
		teacher yteacher;
		yteacher = get_teacher(database, n-1);
		xteacher -> index = yteacher.index + 1;
	}
	else
		xteacher->index = 0;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_teacher");
	fp = fopen(file,"a");
	fprintf(fp,"%d %d %s\n",xteacher->index,xteacher->week_time,xteacher->name);
	fclose(fp);
	return 0;
}
int remove_teacher(char *database, int index){
	FILE *fp;
	int n, i;
	char file[64],temp_file[64];
	n = teacher_number(database);
	teacher xteacher;
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_teacher");
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_teacher");
	fp = fopen(temp_file,"w");
	for(i = 0; i < n; i++) {
		if(i != index) {
			xteacher = get_teacher(database, i);
			fprintf(fp,"%d %d %s\n",xteacher.index,xteacher.week_time, xteacher.name);
		}
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}
int edit_teacher(char *database, int index, teacher *xteacher){
	FILE *fp;
	int n, i;
	char file[64], temp_file[64];
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_teacher");
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_teacher");
	fp = fopen(temp_file,"a");
	n = teacher_number(database);
	teacher yteacher;
	for(i = 0; i < n; i++) {
		yteacher = get_teacher(database, n-1);
		if(i!=index)
			fprintf(fp,"%d %d %s\n",yteacher.index,yteacher.week_time, yteacher.name);
		else
			fprintf(fp,"%d %d %s\n",xteacher->index,xteacher->week_time, xteacher->name);
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}
teacher get_teacher(char *database, int index) {
	FILE *fp;
	int n = 0;
	char file[64];
	teacher xteacher;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_teacher");
	fp = fopen(file,"r");
	while(fscanf(fp,"%d %d %[^\n]s",&xteacher.index, &xteacher.week_time, xteacher.name) != EOF) {
		if(n == index)
			break;
		n++;
	}
	fclose(fp);
	return xteacher;
}
int teacher_number(char *database) {
	FILE *fp;
	int n = 0;
	char file[64];
	teacher xteacher;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_teacher");
	fp = fopen(file,"r");
	if(fp == NULL) {
		fp = fopen(file,"w");
		fclose(fp);
		return 0;
	}
	while(fscanf(fp,"%d %d %[^\n]s",&xteacher.index, &xteacher.week_time, xteacher.name) != EOF)
		n++;
	fclose(fp);
	return n;
}
int sort_teacher(char *database , int(*compare)(const void *x, const void *y)){
	return 0;
}
int show_teacher_info(char *database, int choice) {
	int c;
	teacher xteacher;
	xteacher = get_teacher(database,choice);
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
	printw("Name - %s",xteacher.name);
	move(y/4 + 5,x/3 + 2);
	printw("Max Weekly Hours - %d",xteacher.week_time);
	mvwprintw(win,y - 2, 2,"A:Add Allocation\tB:Back\tQ:Quit");
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
			case 'A':
			case 'a':
				return 2;
			default:
				break;
		}
		wrefresh(win);
	}
	curs_set(1);
	return 0;
}
int start_teacher(char *database){
	int choice, n, sub_choice;
	while(1) {
		choice = teacher_menu(database);
		n = teacher_number(database);
		if(choice == n + 1)
			break;
		else if(choice == n + 2)
			teacher_form(database);
		else if(choice == INT_MIN)
			return INT_MIN;
		else if(choice == -1)
			continue;
		else {
			while(1) {
				sub_choice = show_teacher_info(database,choice);
				if(sub_choice == 1)
					break;
				if(sub_choice == INT_MIN)
					return INT_MIN;
				if(sub_choice == 2)
					alloc_form_teacher(database, choice);
			}
		}
	}
	return 0;
}
int teacher_menu(char *database){
	int i, c, n, choice = 0;
	teacher *xteacher;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	n = teacher_number(database);
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	xteacher = (teacher *)malloc(sizeof(teacher) * (n + 1));
	items = (ITEM **)calloc(n + 3, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		xteacher[i] = get_teacher(database,i);
		items[i] = new_item(xteacher[i].name, NULL);
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
	print_in_middle(win, 1, 0, x, "Teachers" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {
		if(n > 1)
			mvwprintw(win,y - 2, 2,"N:New Teacher\t\tR:Remove Teacher\tS:Sort\t\tB:Back\tQ:Quit");
		else
			mvwprintw(win,y - 2, 2,"N:New Teacher\t\tR:Remove Teacher\tB:Back\tQ:Quit");
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
					remove_teacher(database, choice);
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
		mvwprintw(win,y - 2, 2,"N:New Teacher\t\tB:Back\t\tQ:Quit");
		mvwprintw(win,5,3*x/7,"No Teachers found :(\n");
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
	free(xteacher);
	return -1;
}
int teacher_form(char *database){
	refresh();
	teacher xteacher;
	noecho();
	WINDOW *win;
	int y,x;
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(0, 0, 0, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Enter the Name of Teacher", COLOR_PAIR(1));
	print_form(win,y,x);
	scanstr(win,xteacher.name, x/3 - 1);	
	clear();
	refresh();
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Enter the weekly hours for the teacher", COLOR_PAIR(1));
	print_form(win, y, x);	
	scanw("%d",&xteacher.week_time);
	add_teacher(database, &xteacher);
	refresh();
	endwin();
	clear();
	return 0;
}
