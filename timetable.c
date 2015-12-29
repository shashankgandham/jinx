/*  This file is part of Jinx originally written by Shashank gandham. Timetable Generator is free software: you can redistribute it and/or modify 
    it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version. Timetable Generator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with Jinx.  If not, see <http://www.gnu.org/licenses/>. */

#include "teacher.h"
#include "alloc.h"
#include "subject.h"
#include "batch.h"
#include "room.h"
#include "timetable.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int generate(char *database) {
	alloc *xalloc;
	int n_alloc, i;
	n_alloc = alloc_number(database);
	xalloc = (alloc *)malloc(sizeof(alloc) * (n_alloc + 1));
	for(i = 0; i < n_alloc; i++) 
		xalloc[i] = get_alloc(database,i);
	return 0;
}

int timetable_menu(char *database) {
	int i, c;
	int n = 4;
	ITEM **items;
	MENU *menu;
	WINDOW *win;
	char *choices[] =  {
		"Teachers",
		"Subjects",
		"Batches",
		"Rooms",
	};
	int x, y;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	items = (ITEM **)calloc(n + 2, sizeof(ITEM *));
	for(i = 0; i < n; ++i) {
		items[i] = new_item(choices[i], NULL);
	}
	menu = new_menu((ITEM **)items);
	win = newwin(0, 0, 0, 0);
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,y/2, 1);
	set_menu_mark(menu, " * ");
	set_menu_spacing(menu, 0 , 2, 0);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, database, COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	mvwaddch(win, y - 3, 0, ACS_LTEE);
	mvwhline(win, y - 3, 1, ACS_HLINE, x - 2);
	mvwaddch(win, y - 3, x - 1, ACS_RTEE);
	mvwprintw(win,y - 2, 2,"B:Back\t\tQ:Quit");
	refresh();
	post_menu(menu);
	wrefresh(win);
	int choice = 0;
	while((c = wgetch(win)))
	{       switch(c)
		{	case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			if(choice != n - 1)
				choice++;
			break;
			case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			if(choice != 0)
				choice--;
			break;
			case 10: /* Enter */
				remove_menu(menu, items, n);
				return choice;
				break;
			case 'B':
			case 'b':
				remove_menu(menu, items, n);
				return n+1;
			case 'q':
			case 'Q':
				remove_menu(menu, items, n);
				return INT_MIN;
			default:
			break;
		}
		if(c == 10)
			break;
		wrefresh(win);
	}
	return -1;
}
int start_timetable(char *database) {
	int choice, n = 4;
	while(1) {
		choice = timetable_menu(database);
		if(choice ==  n + 1)
			break;
		else if(choice == INT_MIN)
			return INT_MIN;
		else if(choice == -1)
			continue;
	}
	return 0;
}
long random_int(unsigned min, unsigned max) {
	int r;
	srand(time(NULL));
	while((r = rand()%max)) {
		if(r > min)
			return r;
	}
	return r;
}
void timetable_integrity (timetable *xtimetable , info *xinfo) {
	int i;
	for(i = 0; i < xinfo->slot_number; i++) {
		assert(xtimetable->time[xtimetable->slot[i]] == i);
	}
}
void timetable_init(timetable *xtimetable,  info *xinfo) {
	xtimetable->time = (int *)malloc(sizeof(int) * xinfo->day_slot * xinfo->room_number);	
	xtimetable->fitness = 0;
	xtimetable->slot = (int *)malloc(sizeof(slot) * xinfo->slot_number);	
	int i;
	for(i = 0; i < (xinfo->day_slot * xinfo->room_number); i++)
		xtimetable->time[i] = -1;
}
void timetable_gen(timetable *xtimetable, info *xinfo) {
	int i, slot, empty;
	for(i = 0; i < xinfo->slot_number; i++) {
		slot = random_int(0, (xinfo->room_number * xinfo->day_slot));
		empty = timetable_check_empty(xtimetable, slot, xinfo);
		xtimetable->time[empty] = i;
		xtimetable->slot[i] = empty;
	}
	timetable_integrity(xtimetable, xinfo);
	xtimetable->fitness = fitness(xtimetable, xinfo);
}
int compare(const void *a, const void *b) {
	if((*(timetable *)a).fitness > (*(timetable *)b).fitness)
		return 1;
	else if((*(timetable *)a).fitness < (*(timetable *)b).fitness)
		return -1;
	else
		return 0;
}
void population_sort(population *xpopulation, info *xinfo) {
	qsort(xpopulation->xtimetable, xinfo->population_number,sizeof(timetable),compare);
}
void population_gen(population *xpopulation, info *xinfo) {
	xpopulation->generation = 0;
	xpopulation->xtimetable = (timetable *)malloc(sizeof(timetable) * xinfo->population_number);
	
	int i;
