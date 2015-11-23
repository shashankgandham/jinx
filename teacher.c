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
int *find_teacher_info(char *database, int index){
}
int sort_teacher(char *database , int(*compare)(const void *x ,const void *y)){

	return 0;
}
int start_teacher(char *database){
	int choice, n, sub_choice;
	while(1) {
		choice = teacher_menu(database);
		n = teacher_number(database) + 2;
		if(choice == n - 1)
			break;
		else if(choice == n - 2)
			teacher_form(database);
		else {
			sub_choice = teacher_submenu(database, choice);
		}
	}
	return 0;
}
int teacher_submenu(char *database,int index){
	int i, c, n = 2, choice;
	char *choices[] = {
		"Remove Teacher",
		"Edit Teacher",
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
	print_in_middle(win, 1, 0, x,get_teacher(database, index).name, COLOR_PAIR(1));
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
int teacher_menu(char *database){
	int i, c, n, choice;
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
	items[i] = new_item("Add Teacher",NULL);
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
				break;
			
			default:
				break;
		}
		if(c == 10)
			break;
		wrefresh(win);
	}	
	free(xteacher);
	return choice;
}
int teacher_form(char *database){
	refresh();
	teacher xteacher;
	echo();
	WINDOW *win;
	int y,x;	
	start_color();
	getmaxyx(stdscr,y,x);
	win = newwin(6, 40, y/3, x/3);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, 40, "Enter the Name of Teacher", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw(" %[^\n]s",xteacher.name);
	clear();
	box(win, 0, 0);
	print_in_middle(win, 1, 0, 40, "Enter the weekly hours for the teacher", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw("%d",&xteacher.week_time);
	add_teacher(database, &xteacher);
	refresh();
	endwin();
	clear();
	return 0;
}
