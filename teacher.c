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
	char file[16];
	n = teacher_number(database);
	if(n) {
		teacher *yteacher;
		yteacher = (teacher *)malloc(sizeof(teacher));
		*yteacher = get_teacher(database, n-1);
		xteacher -> index = yteacher->index + 1;
		free(yteacher);
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
	int n, i , k = 0;
	char file[16];
	n = teacher_number(database);
	teacher *xteacher;
	xteacher = (teacher *)malloc(sizeof(teacher) * n);
	for(i = 0; i < n; i++) {
		if(i != index) {
			xteacher[k] = get_teacher(database, n-1);
			k++;
		}
	}
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_teacher");
	fp = fopen(file,"a");
	for(i = 0; i < n - 1; i++)
		fprintf(fp,"%d %d %s\n",xteacher[i].index,xteacher[i].week_time, xteacher[i].name);
	fclose(fp);
	free(xteacher);
	return 0;
}
int edit_teacher(char *database, int index, char *name){
	FILE *fp;
	int n, i , k = 0;
	char file[16];
	n = teacher_number(database);
	teacher *xteacher;
	xteacher = (teacher *)malloc(sizeof(teacher) * n);
	for(i = 0; i < n; i++) {
		xteacher[k] = get_teacher(database, n-1);
		k++;
	}
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_teacher");
	strcpy(xteacher[index].name, name);
	fp = fopen(file,"a");
	for(i = 0; i < n; i++)
		fprintf(fp,"%d %d %s\n",xteacher[i].index,xteacher[i].week_time, xteacher[i].name);
	fclose(fp);
	free(xteacher);
	return 0;
}
teacher get_teacher(char *database, int index) {
	FILE *fp;
	int n = 0;
	char file[16];
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
	char file[16];
	teacher xteacher;
	strcpy(file, "Database/");
	strcat(file, database);
	strcat(file, "_teacher");
	fp = fopen(file,"r");
	while(fscanf(fp,"%d %d %[^\n]s",&xteacher.index, &xteacher.week_time, xteacher.name) != EOF)
		n++;
	fclose(fp);
	return n;
}
alloc *find_teacher_info(char *database, int index){
	alloc *xalloc;
	xalloc = (alloc *)malloc(sizeof(alloc));
	return xalloc;
}
int sort_teacher(char *database , int(*compare)(const void *x ,const void *y)){

	return 0;
}
int start_teacher(char *database){
	int n, total;
	while(1) {
		n = teacher_menu(database);
		total = teacher_number(database) + 2;
		if(n == total - 1)
			break;
	}
	return 0;
}
int teacher_menu(char *database){
	char teacher_choices[128][128];
	int i, c, n, choice;
	WINDOW *win;
	ITEM **items;
	MENU *menu;
	n = teacher_number(database);
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);	
	items = (ITEM **)calloc(n + 1, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		items[i] = new_item(teacher_choices[i], NULL);
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
	return 1;

}
int teacher_form(char *database){

	return 0;
}
