/*    
    This file is part of Xtimetable originally written by Shashank Gandham. Xtimetable is free software: you can redistribute it and/or 
    modifyvit under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, 
    or (at your option) any later version.Xtimetable is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
    details. You should have received a copy of the GNU General Public License along with Xtimetable.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
int sort_subject(char *database , int(*compare)(const void *x ,const void *y)){
	return 0;
}
int show_subject_info(char *database, int choice) {
	int c;
	subject xsubject;
	xsubject = get_subject(database,choice);
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
	printw("Name - %s",xsubject.name);
	move(y/4 + 5,x/3 + 2);
	printw("Slot Length - %d",xsubject.slot_len);
	move(y/4 + 7,x/3 + 2);
	printw("No. of slots per week - %d",xsubject.slot_per_week);
	mvwprintw(win,y - 2, 2,"A:Add Allocation\t\tB:Back\tQ:Quit");
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
				curs_set(1);
				return 2;
			default:
				break;
		}
	}
	return 0;
}
int start_subject(char *database){
	int choice, n, sub_choice;
	while(1) {
		choice = subject_menu(database);
		n = subject_number(database);
		if(choice == n + 1)
			break;
		else if(choice == n + 2)
			subject_form(database);
		else if(choice == INT_MIN)
			return INT_MIN;
		else if(choice == -1)
			continue;
		else {
			while(1) {
				sub_choice = show_subject_info(database, choice);
				if(sub_choice == 1)
					break;
				if(sub_choice == INT_MIN)
					return INT_MIN;
				if(sub_choice == 2)
					alloc_form_subject(database, choice);
			}
		}
	}
	return 0;
}
int subject_menu(char *database){
	int i, c, n, choice = 0;
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
	print_in_middle(win, 1, 0, x, "Subjects" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {
		if(n > 1)
			mvwprintw(win,y - 2, 2,"N:New Subject\t\tR:Remove Subject\tS:Sort\t\tB:Back\tQ:Quit");
		else
			mvwprintw(win,y - 2, 2,"N:New Subject\t\tR:Remove Subject\tB:Back\tQ:Quit");
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
					remove_subject(database, choice);
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
		mvwprintw(win,y - 2, 2,"N:New Subject\t\tB:Back\t\tQ:Quit");
		mvwprintw(win,5,3*x/7,"No Subjects found :(\n");
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
	free(xsubject);
	return -1;
}
int subject_form(char *database){
	refresh();
	subject xsubject;
	noecho();
	WINDOW *win;
	int y,x;
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(0, 0, 0, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Enter the Name of Subject", COLOR_PAIR(1));
	print_form_str(win, y,x);	
	scanstr(win,xsubject.name,x/3 - 1);
	clear();
	refresh();
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Enter the length of single slot ", COLOR_PAIR(1));
	print_form_str(win, y, x);	
	xsubject.slot_len = scanint(win);
	clear();
	refresh();
	box(win, 0, 0);
	print_in_middle(win, y/4 + 1, 0, x, "Enter the number of slots per week ", COLOR_PAIR(1));
	print_form_str(win, y,x);	
	xsubject.slot_per_week = scanint(win);
	add_subject(database, &xsubject);
	refresh();
	endwin();
	clear();
	return 0;
}
