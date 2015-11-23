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

#include "subject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_subject(char *database, subject *xsubject) {
	FILE *fp;
	int n;
	char file[64];
	n = subject_number(database);
	if(n) {
		subject ysubject;
		ysubject = get_subject(database, n-1);
		xsubject -> index = ysubject.index + 1;
	}
	else
		xsubject->index = 0;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_subject");
	fp = fopen(file,"a");
	fprintf(fp,"%d %d %d %s\n",xsubject->index,xsubject->slot_len,xsubject->slot_per_week,xsubject->name);
	fclose(fp);
	return 0;
}
int remove_subject(char *database, int index){
	FILE *fp;
	int n, i;
	char file[64],temp_file[64];
	n = subject_number(database);
	subject xsubject;	
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_subject");
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_subject");
	fp = fopen(temp_file,"w");
	for(i = 0; i < n; i++) {
		if(i != index) {
			xsubject = get_subject(database, i);
			fprintf(fp,"%d %d %d %s\n",xsubject.index,xsubject.slot_len,xsubject.slot_per_week ,xsubject.name);
		}
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}
int edit_subject(char *database, int index, subject *xsubject){
	FILE *fp;
	int n, i;
	char file[64], temp_file[64];
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_subject");
	strcpy(temp_file, "Database/");
	strcat(temp_file, ".");
	strcat(temp_file, database);
	strcat(temp_file, "_subject");
	fp = fopen(temp_file,"a");
	n = subject_number(database);
	subject ysubject;
	for(i = 0; i < n; i++) {
		ysubject = get_subject(database, n-1);
		if(i!=index)
			fprintf(fp,"%d %d %d %s\n",ysubject.index,ysubject.slot_len,ysubject.slot_per_week, ysubject.name);
		else
			fprintf(fp,"%d %d %d %s\n",xsubject->index,xsubject->slot_len,xsubject->slot_per_week, xsubject->name);
	}
	remove(file);
	rename(temp_file,file);
	fclose(fp);
	return 0;
}
subject get_subject(char *database, int index) {
	FILE *fp;
	int n = 0;
	char file[64];
	subject xsubject;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_subject");
	fp = fopen(file,"r");
	while(fscanf(fp,"%d %d %d %[^\n]s",&xsubject.index, &xsubject.slot_len,&xsubject.slot_per_week,xsubject.name) != EOF) {
		if(n == index)
			break;
		n++;
	}
	fclose(fp);
	return xsubject;
}
int subject_number(char *database) {
	FILE *fp;
	int n = 0;
	char file[64];
	subject xsubject;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_subject");
	fp = fopen(file,"r");
	if(fp == NULL) {
		fp = fopen(file,"w");
		fclose(fp);
		return 0;
	}
	while(fscanf(fp,"%d %d %d %[^\n]s",&xsubject.index, &xsubject.slot_len, &xsubject.slot_per_week, xsubject.name) != EOF)
		n++;
	fclose(fp);
	return n;
}
int *find_subject_info(char *database, int index){
}
int sort_subject(char *database , int(*compare)(const void *x ,const void *y)){

	return 0;
}
int start_subject(char *database){
	int n, choice, sub_choice;
	while(1) {
		choice = subject_menu(database);
		n = subject_number(database) + 2;
		if(choice == n - 1)
			break;
		if(choice == n - 2)
			subject_form(database);
		else {
			sub_choice = subject_submenu(database, choice);
		}
	}
	return 0;
}
int subject_submenu(char *database,int index){
	int i, c, n = 2, choice;
	char *choices[] = {
		"Remove Subject",
		"Edit Subject",
	};
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);	
	items = (ITEM **)calloc(n + 2, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		items[i] = new_item(choices[i], NULL);
	}
	items[i] = new_item("Back",NULL);
	n+=1;
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
	print_in_middle(win, 1, 0, x,get_subject(database, index).name, COLOR_PAIR(1));
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
	return choice;
}

int subject_menu(char *database){
	int i, c, n, choice;
	subject *xsubject;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	n = subject_number(database);
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);	
	xsubject = (subject *)malloc(sizeof(subject) * (n + 1));
	items = (ITEM **)calloc(n + 3, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		xsubject[i] = get_subject(database,i);
		items[i] = new_item(xsubject[i].name, NULL);
	}
	items[i] = new_item("Add Subject",NULL);
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
	print_in_middle(win, 1, 0, x, "Subjects" , COLOR_PAIR(1));
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
	free(xsubject);
	return choice;
}
int subject_form(char *database){
	refresh();
	subject xsubject;
	echo();
	WINDOW *win;
	int y,x;	
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(6, 40, y/3, x/3);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, 40, "Enter the Name of Subject", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw(" %[^\n]s",xsubject.name);
	clear();
	box(win, 0, 0);
	print_in_middle(win, 1, 0, 40, "Length of a single slot for the Subject", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw("%d",&xsubject.slot_len);
	clear();
	box(win, 0, 0);
	print_in_middle(win, 1, 0, 40, "Enter the number of such slots in a week", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw("%d",&xsubject.slot_per_week);
	add_subject(database, &xsubject);
	refresh();
	endwin();
	clear();
	return 0;
}
