/*
	MIT LICENSE Copyright (c) 2016 Shashank Gandham
*/

#include "library.h"
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
	char file[63],temp_file[64];
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
	while(fscanf(fp,"%d %d %d %d %d",&xalloc.index, &xalloc.batch, &xalloc.subject, &xalloc.teacher, &xalloc.slot) != EOF) {
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
	while(fscanf(fp,"%d %d %d %d %d",&xalloc.index, &xalloc.batch, &xalloc.subject, &xalloc.teacher,&xalloc.slot) != EOF)
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
int show_alloc_info(char *database, int choice) {
	int c;
	alloc xalloc;
	teacher xteacher;
	subject xsubject;
	batch xbatch;
	xalloc = get_alloc(database, choice);
	xteacher = get_teacher(database,get_array_index(database,2,xalloc.teacher));
	xbatch = get_batch(database,get_array_index(database,0,xalloc.batch));
	xsubject = get_subject(database,get_array_index(database,1,xalloc.subject));
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
	printw("Batch - %s",xbatch.name);
	move(y/4 + 5,x/3 + 2);
	printw("Subject - %s",xsubject.name);
	move(y/4 + 7,x/3 + 2);
	printw("Teacher - %s",xteacher.name);
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
int start_alloc(char *database){
	int choice, n, sub_choice;
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
		else{
			while(1) {
				sub_choice = show_alloc_info(database, choice);
				if(sub_choice == 1)
					break;
				if(sub_choice == INT_MIN)
					return INT_MIN;
			}
		}
	}
	return 0;
}
int alloc_menu(char *database){
	int i, c, n, choice = 0;
	alloc xalloc;
	batch xbatch;
	subject xsubject;
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
	for(i = 0; i <= n; ++i) {
		choices[i] = (char *)malloc(sizeof(char) * 64);
		description[i] = (char *)malloc(sizeof(char) * 128);
		if(i) {
			xalloc = get_alloc(database,i - 1);
			xbatch = get_batch(database, get_array_index(database, 0, xalloc.batch));
			strcpy(choices[i],xbatch.name);
			xsubject = get_subject(database, get_array_index(database, 1, xalloc.subject));
			strcpy(description[i],xsubject.name);
		}
		else {
			strcpy(choices[i],"Batch");
			strcpy(description[i],"Subject");
		}
		items[i] = new_item(choices[i], description[i]);
	}
	menu = new_menu((ITEM **)items);
	win = newwin(0, 0, 0, 0);
	int y,x;
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 0.5*x, 5, 0.4*x));
	set_menu_format(menu, x - 4 , 1);
	set_menu_mark(menu, " * ");
	set_menu_spacing(menu, 8, 2, 0);
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
		menu_driver(menu, REQ_DOWN_ITEM);
		while((c = wgetch(win))){
			switch(c) {
				case KEY_DOWN:
					menu_driver(menu, REQ_DOWN_ITEM);
					if(choice != n -1)
						choice++;
					break;
				case KEY_UP:
					if(choice != 0) {
						menu_driver(menu, REQ_UP_ITEM);
						choice--;
					}
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
		mvwprintw(win,y - 2, 2,"A:Allocate to a new Teacher\tEnter:Select\tB:Back\tQ:Quit");
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
				case 'A':
				case 'a':
					return n + 2;
					break;
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
	else {
		mvwprintw(win,y - 2, 2,"A:Allocate to a new Teacher\tB:Back\t\tQ:Quit");
		mvwprintw(win,5,3*x/7,"No Teachers found\n");
		wrefresh(win);
		curs_set(0);
		while((c = wgetch(win)))
		{
			switch(c) {
				case 'A':
				case 'a':
					curs_set(1);
					return n + 2;
					break;
				case 'b':
				case 'B':
					remove_menu(menu,items,n);
					curs_set(1);
					return n +1;
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
	print_in_middle(win, 1, 0, x, "Subjects" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {
		mvwprintw(win,y - 2, 2,"A:Allocate a new Subject\tB:Back\t\tQ:Quit");
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
				case 'A':
				case 'a':
					return n + 2;
					break;
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
	else {
		mvwprintw(win,y - 2, 2,"A:Allocate a new Subject\tB:Back\t\tQ:Quit");
		mvwprintw(win,5,3*x/7,"No Subjects found\n");
		wrefresh(win);
		curs_set(0);
		while((c = wgetch(win)))
		{
			switch(c) {
				case 'A':
				case 'a':
					curs_set(1);
					return n + 2;
					break;
				case 'b':
				case 'B':
					remove_menu(menu,items,n);
					curs_set(1);
					return n + 1;
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
	print_in_middle(win, 1, 0, x, "Batches" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	refresh();
	if(n) {
		mvwprintw(win,y - 2, 2,"A:Allocate to new Batch\tB:Back\t\tQ:Quit");
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
				case 'A':
				case 'a':
					return n + 2;
					break;
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
	else {
		mvwprintw(win,y - 2, 2,"A:Allocate to new Batch\tB:Back\t\tQ:Quit");
		mvwprintw(win,5,3*x/7,"No Batches found\n");
		wrefresh(win);
		curs_set(0);
		while((c = wgetch(win))) {
			switch(c) {
				case 'b':
				case 'B':
					remove_menu(menu,items,n);
					curs_set(1);
					return n + 1;
				case 'Q':
				case 'q':
					remove_menu(menu,items,n);
					curs_set(1);
					return INT_MIN;
				case 'A':
				case 'a':
					curs_set(1);
					return n + 2;
					break;
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
	return -1;
}
int get_array_index(char *database, int datum, int index) {
	int i, n;
	if(datum == 2) {
		n = teacher_number(database);
		for(i = 0; i < n; i++) {
			if(get_teacher(database, i).index == index)
				return i;
		}
	}
	if(datum == 0) {
		n = batch_number(database);
		for(i = 0; i < n; i++) {
			if(get_batch(database, i).index == index)
				return i;
		}
	}
	if(datum == 1) {
		n = subject_number(database);
		for(i = 0; i < n; i++) {
			if(get_subject(database, i).index == index)
				return i;
		}
	}
	if(datum == 3) {
		n = teacher_number(database);
		for(i = 0; i < n; i++) {
			if(get_room(database, i).index == index)
				return i;
		}
	}
	return -1;
}
int alloc_form(char *database) {
	alloc xalloc;
	int n;
	n = alloc_batch(database);
	if(n == batch_number(database) + 1)
		return 1;
	if(n == batch_number(database) + 2) {
		batch_form(database);
		n-=1;
	}
	xalloc.batch = get_index(database, 0, n);
	n = alloc_subject(database);
	if(n == subject_number(database) + 1)
		return 1;
	if(n == subject_number(database) + 2) {
		subject_form(database);
		n-=1;
	}
	xalloc.subject = get_index(database, 1, n);
	n = alloc_teacher(database);
	if(n == teacher_number(database) + 1)
		return 1;
	if(n == teacher_number(database) + 2) {
		teacher_form(database);
		n-=1;
	}
	xalloc.teacher = get_index(database, 2, n);
	xalloc.slot = 0;	
	add_alloc(database, &xalloc);
	return 0;
}
int alloc_form_teacher(char *database, int index) {
	alloc xalloc;
	int n;
	n = alloc_batch(database);
	if(n == batch_number(database) + 1)
		return 1;
	if(n == batch_number(database) + 2) {
		batch_form(database);
		n-=1;
	}
	xalloc.batch = get_index(database, 0, n);
	n = alloc_subject(database);
	if(n == subject_number(database) + 1)
		return 1;
	if(n == subject_number(database) + 2) {
		subject_form(database);
		n-=1;
	}
	xalloc.subject = get_index(database, 1, n);
	xalloc.teacher = get_index(database, 2, index);
	add_alloc(database, &xalloc);
	return 0;
}
int alloc_form_subject(char *database, int index) {
	alloc xalloc;
	int n;
	n = alloc_batch(database);
	if(n == batch_number(database) + 1)
		return 1;
	if(n == batch_number(database) + 2) {
		batch_form(database);
		n-=1;
	}
	xalloc.batch = get_index(database, 0, n);
	xalloc.subject = get_index(database, 1, index);
	n = alloc_teacher(database);
	if(n == -1 || n == teacher_number(database) + 1)
		return 1;
	if(n == teacher_number(database) + 2) {
		teacher_form(database);
		n-=1;
	}
	xalloc.teacher = get_index(database, 2, n);
	add_alloc(database, &xalloc);
	return 0;
}
int alloc_form_batch(char *database, int index) {
	alloc xalloc;
	int n;
	xalloc.batch = get_index(database, 0, index);
	n = alloc_subject(database);
	if(n == subject_number(database) + 1)
		return 1;
	if(n == subject_number(database) + 2) {
		subject_form(database);
		n-=1;
	}
	xalloc.subject = get_index(database, 1, n);
	n = alloc_teacher(database);
	if(n == -1 || n == teacher_number(database) + 1)
		return 1;
	if(n == teacher_number(database) + 2) {
		teacher_form(database);
		n-=1;
	}
	xalloc.teacher = get_index(database, 2, n);
	add_alloc(database, &xalloc);
	return 0;
}
