/*  This file is part of Timetable generator originally written by Shashank gandham.

    Timetable Generator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Timetable Generator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Timetable generator.  If not, see <http://www.gnu.org/licenses/>.
    */
#include <menu.h>
#include "library.h"

char **main_menu_choices;
ITEM **items;
int main_menu_choice, database_menu_choice, data_menu_choice, slot_menu_choice, edit_menu_choice;
MENU *menu;
WINDOW *win;;
int n_main_menu, n_database_menu, n_data_menu,n_edit_menu, n_slot_menu;
void remove_menu(int n) {
	int i;
	unpost_menu(menu);
	free_menu(menu);
	for(i = 0; i < n; ++i)
		free_item(items[i]);
	endwin();
	clear();
}
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}
int get_menu_choices() {
	FILE *fp;
	fp = fopen("database.txt","r");
	int i;
	main_menu_choices = (char **)malloc(sizeof(char *) *128);
	for(i = 0; i < 128; i++)
		main_menu_choices[i] = (char *)malloc(sizeof(char) *128); 
	if(fp == NULL) {
		fp = fopen("database.txt","w");
		fprintf(fp,"%s\n","New database");
		fprintf(fp,"%s\n","Exit");
		fclose(fp);
	}
	fp = fopen("database.txt","r");
	i = 0;
	while(fscanf(fp," %[^\n]s",main_menu_choices[i])!= EOF) {
		if(!strcmp("Exit",main_menu_choices[i])) {
			i++;
			break;
		}
		i++;
	}
	fclose(fp);
	return i;
}
int main_menu() {
	int i, c, x, y;
	n_main_menu = get_menu_choices();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	items = (ITEM **)calloc(n_main_menu + 2, sizeof(ITEM *));
	for(i = 0; i < n_main_menu; i++) {
		items[i] = new_item(main_menu_choices[i], NULL);
	}	
	menu = new_menu((ITEM **)items);
	win = newwin(0, 0, 0, 0);
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, "My Menu", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	refresh(); 
	post_menu(menu);
	wrefresh(win);
	main_menu_choice = 0;
	while((c = wgetch(win)))
	{       
		switch(c)
		{	case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			if(main_menu_choice != (n_main_menu - 1))
				main_menu_choice++;
			break;
			case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			if(main_menu_choice != 0)
				main_menu_choice--;
			break;
			case 10:
			remove_menu(n_main_menu);
			return main_menu_choice;
			default:
			break;
		}
		wrefresh(win);
	}	
	return -1;
}
void database_menu() {
	int n;
	while(1){
		n = show_database_menu();
		if(n == 6) {
			remove_database(main_menu_choice);
			remove_files(main_menu_choices[main_menu_choice]);
			return;
		}
		if(n == 7)
			return;
		else if(n == 0) 
			show_teacher();
		else if(n == 1)
			show_subject();
		else if(n == 2)
			show_class();
		else if(n == 3)
			show_room();
		else if(n == 4)
			show_slot();
		else if(n == 5)
			generate_timetable();
	}
}
int main() {
	int n;
	initscr();
	start_color();
	cbreak();
	noecho();
	while(1) {
		n = main_menu();
		if(n == n_main_menu - 1) {
			endwin();
			return 0;
		}
		else if(n == n_main_menu - 2)
			new_database_form();
		else
			database_menu();
	}
	return 0;
}
void new_database_form() {
	refresh();
	char name[128];
	echo();
	int y,x;	
	start_color();
	getmaxyx(win,y,x);
	win = newwin(6, 40, y/3, x/3);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	print_in_middle(win, 1, 0, 40, "Enter the name of database", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw(" %[^\n]s",name);
	new_database(name);
	refresh();
	endwin();
	clear();
	return;
}

int show_database_menu() {		
	int i, c;
	n_database_menu = 8;
	char *database_menu_choices[] =  {
		"Teachers",
		"Subjects",
		"Classes",
		"Rooms",
		"Slots",
		"Make Timetable",
		"Remove Database",
		"Back",
	};
	int x, y;	
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	items = (ITEM **)calloc(n_database_menu + 2, sizeof(ITEM *));
	for(i = 0; i < n_database_menu; ++i) {
		items[i] = new_item(database_menu_choices[i], NULL);
	}	
	menu = new_menu((ITEM **)items);
	win = newwin(0, 0, 0, 0);
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, main_menu_choices[main_menu_choice], COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	refresh();
	post_menu(menu);
	wrefresh(win);
	database_menu_choice = 0;
	while((c = wgetch(win)))
	{       switch(c)
		{	case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			if(database_menu_choice != n_database_menu - 1)
				database_menu_choice++;
			break;
			case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			if(database_menu_choice != 0)
				database_menu_choice--;
			break;
			case 10: /* Enter */
			remove_menu(n_database_menu);
			return database_menu_choice;
			default:
			break;
		}
		wrefresh(win);
	}	
	return -1;
}

void add_data_form() {
	refresh();
	char name[128];
	echo();	
	start_color();
	int x, y;
	getmaxyx(win,y,x);
	win = newwin(6, 40,y/3,x/3);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	if(database_menu_choice == 0)
		print_in_middle(win, 1, 0, 40, "Enter the name of Teacher", COLOR_PAIR(1));
	if(database_menu_choice == 1)
		print_in_middle(win, 1, 0, 40, "Enter the name of Subject", COLOR_PAIR(1));
	if(database_menu_choice == 2)
		print_in_middle(win, 1, 0, 40, "Enter the name of Class", COLOR_PAIR(1));
	if(database_menu_choice == 3)
		print_in_middle(win, 1, 0, 40, "Enter the name of Room", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 +2);
	scanw(" %[^\n]s",name);
	if(database_menu_choice == 0)
		add_teacher(main_menu_choices[main_menu_choice],name);
	if(database_menu_choice == 1)
		add_subject(main_menu_choices[main_menu_choice],name);
	if(database_menu_choice == 2)
		add_class(main_menu_choices[main_menu_choice],name);
	if(database_menu_choice == 3)
		add_room(main_menu_choices[main_menu_choice],name);

	refresh();
	endwin();
	clear();
}
int edit_data_menu() {
	int i, c;
	n_edit_menu = 3;
	char *edit_menu_choices[] =  {
		"Edit Name",
		"Remove",
		"Back",
	};
	int x, y;	
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	items = (ITEM **)calloc(n_edit_menu + 2, sizeof(ITEM *));
	for(i = 0; i < n_edit_menu; ++i) {
		items[i] = new_item(edit_menu_choices[i], NULL);
	}	
	menu = new_menu((ITEM **)items);
	win = newwin(0, 0, 0, 0);
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x,"Edit Menu" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 1);
	mvwaddch(win, 2, x - 2, ACS_RTEE);
	refresh();
	post_menu(menu);
	wrefresh(win);
	edit_menu_choice = 0;
	while((c = wgetch(win)))
	{       switch(c)
		{	case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			if(edit_menu_choice != n_edit_menu - 1)
				edit_menu_choice++;
			break;
			case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			if(edit_menu_choice != 0)
				edit_menu_choice--;
			break;
			case 10: /* Enter */
			remove_menu(n_edit_menu);
			return edit_menu_choice;
			default:
			break;
		}
		wrefresh(win);
	}	
	return -1;
}
void edit_data_form(char *database, int marker, int n) {
	refresh();
	char name[128];
	echo();	
	int y,x;
	start_color();
	getmaxyx(win,y,x);
	win = newwin(6, 40, y/3, x/3);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	box(win, 0, 0);
	if(database_menu_choice == 0)
		print_in_middle(win, 1, 0, 40, "Enter the name of Teacher", COLOR_PAIR(1));
	if(database_menu_choice == 1)
		print_in_middle(win, 1, 0, 40, "Enter the name of Subject", COLOR_PAIR(1));
	if(database_menu_choice == 2)
		print_in_middle(win, 1, 0, 40, "Enter the name of Class", COLOR_PAIR(1));
	if(database_menu_choice == 3)
		print_in_middle(win, 1, 0, 40, "Enter the name of Room", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	wrefresh(win);
	move(y/3 + 3,x/3 + 2);
	scanw(" %[^\n]s",name);
	if(marker == TEACHER)
		edit_teacher(database,name, n);
	if(marker == SUBJECT)
		edit_subject(database,name, n);
	if(marker == CLASS)
		edit_class(database,name, n);
	if(marker== ROOM)
		edit_room(database,name, n);

	refresh();
	endwin();
	clear();
}
void data_menu(char *database, int marker) {
	int n, c;
	while(1) {
		n = display_data(database, marker);
		if(n == n_data_menu - 1)
			return;
		if(n == n_data_menu - 2)
			add_data_form();
		else if ( n < n_data_menu - 2){
			c = edit_data_menu();
			if(c == 1) {
				if(marker == TEACHER)
					remove_teacher(database, n);
				if(marker == ROOM)
					remove_room(database, n);
				if(marker == SUBJECT)
					remove_subject(database, n);
				if(marker == CLASS)
					remove_class(database, n);
				char file[128];
				strcpy(file,database);
				strcat(file,"_slot.dat");
				remove(file);
			}	
			else if(c == 0) {
				edit_data_form(database, marker, n);
			}
			if(c == 2)
				continue;
		}
	}
}
int display_data(char *database, int marker) {
	char data_menu_choices[128][128];
	int i, c;
	data xdata[128];
	if(marker == TEACHER) {	
		n_data_menu = read_teacher(database, xdata);
		strcpy(data_menu_choices[n_data_menu],"Add Teacher");
	}	
	if(marker == CLASS) {
		n_data_menu = read_class(database, xdata);
		strcpy(data_menu_choices[n_data_menu],"Add Class");
	}	
	if(marker == SUBJECT) {
		n_data_menu = read_subject(database, xdata);
		strcpy(data_menu_choices[n_data_menu],"Add Subject");
	}	
	if(marker == ROOM) {
		n_data_menu = read_room(database, xdata);
		strcpy(data_menu_choices[n_data_menu],"Add Room");
	}
	for(i = 0; i < n_data_menu; i++) {
		strcpy(data_menu_choices[i],xdata[i].name);
	}
	strcpy(data_menu_choices[n_data_menu + 1],"Back");
	n_data_menu+=2;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);	
	items = (ITEM **)calloc(n_data_menu + 1, sizeof(ITEM *));
	for(i = 0; i < n_data_menu; ++i) {
		items[i] = new_item(data_menu_choices[i], NULL);
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
	if(marker == TEACHER) 
		print_in_middle(win, 1, 0, x, "Teachers" , COLOR_PAIR(1));
	if(marker == CLASS) 
		print_in_middle(win, 1, 0, x, "Classes" , COLOR_PAIR(1));
	if(marker == SUBJECT) 
		print_in_middle(win, 1, 0, x, "Subjects" , COLOR_PAIR(1));
	if(marker == ROOM) 
		print_in_middle(win, 1, 0, x, "Rooms" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	refresh();
	post_menu(menu);
	wrefresh(win);
	data_menu_choice = 0;
	while((c = wgetch(win)))
	{       switch(c)
		{	case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			if(data_menu_choice != n_data_menu -1)
				data_menu_choice++;
			break;
			case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			if(data_menu_choice != 0)
				data_menu_choice--;
			break;
			case 10: /* Enter */
			remove_menu(n_data_menu);
			return data_menu_choice;
			break;
			default:
			break;
		}
		wrefresh(win);
	}	
	return -1;
}
void show_teacher() {
	char database[128];
	strcpy(database,main_menu_choices[main_menu_choice]);
	data_menu(database, TEACHER);
}
void show_class() {	
	char database[128];
	strcpy(database,main_menu_choices[main_menu_choice]);
	data_menu(database, CLASS);
}
void show_subject() {	
	char database[128];
	strcpy(database,main_menu_choices[main_menu_choice]);
	data_menu(database, SUBJECT);
}
void show_room() {	
	char database[128];
	strcpy(database,main_menu_choices[main_menu_choice]);
	data_menu(database, ROOM);
}

int add_slot_teacher(char *database) {
	char data_menu_choices[128][128];
	int i, c;
	data xdata[128];
	n_data_menu = read_teacher(database, xdata);	
	for(i = 0; i < n_data_menu; i++) {
		strcpy(data_menu_choices[i],xdata[i].name);
	}
	strcpy(data_menu_choices[n_data_menu],"Back");
	n_data_menu+=1;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);	
	items = (ITEM **)calloc(n_data_menu + 1, sizeof(ITEM *));
	for(i = 0; i < n_data_menu; ++i) {
		items[i] = new_item(data_menu_choices[i], NULL);
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
	data_menu_choice = 0;
	while((c = wgetch(win)))
	{       switch(c)
		{	case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			if(data_menu_choice != n_data_menu - 1)
				data_menu_choice++;
			break;
			case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			if(data_menu_choice != 0)
				data_menu_choice--;
			break;
			case 10: /* Enter */
			remove_menu(n_data_menu);
			return data_menu_choice;
			break;
			default:
			break;
		}
		wrefresh(win);
	}	
	return -1;
}
int add_slot_subject(char *database) {
	char data_menu_choices[128][128];
	int i, c;
	data xdata[128];
	n_data_menu = read_subject(database, xdata);	
	for(i = 0; i < n_data_menu; i++) {
		strcpy(data_menu_choices[i],xdata[i].name);
	}
	strcpy(data_menu_choices[n_data_menu],"Back");
	n_data_menu+=1;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);	
	items = (ITEM **)calloc(n_data_menu + 1, sizeof(ITEM *));
	for(i = 0; i < n_data_menu; ++i) {
		items[i] = new_item(data_menu_choices[i], NULL);
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
	print_in_middle(win, 1, 0, x, "Subjects" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x-2);
	mvwaddch(win, 2, x-1, ACS_RTEE);
	refresh();
	post_menu(menu);
	wrefresh(win);
	data_menu_choice = 0;
	while((c = wgetch(win)))
	{       switch(c)
		{	case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			if(data_menu_choice != n_data_menu -1)
				data_menu_choice++;
			break;
			case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			if(data_menu_choice != 0)
				data_menu_choice--;
			break;
			case 10: /* Enter */
			remove_menu(n_data_menu);
			return data_menu_choice;
			break;
			default:
			break;
		}
		wrefresh(win);
	}	
	return -1;
}
int add_slot_class(char *database) {
	char data_menu_choices[128][128];
	int i, c;
	data xdata[128];
	n_data_menu = read_class(database, xdata);	
	for(i = 0; i < n_data_menu; i++) {
		strcpy(data_menu_choices[i],xdata[i].name);
	}
	strcpy(data_menu_choices[n_data_menu],"Back");
	n_data_menu+=1;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);	
	items = (ITEM **)calloc(n_data_menu + 1, sizeof(ITEM *));
	for(i = 0; i < n_data_menu; ++i) {
		items[i] = new_item(data_menu_choices[i], NULL);
	}	
	menu = new_menu((ITEM **)items);
	int y, x;
	win = newwin(0, 0, 0, 0);
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	box(win, 0, 0);	
	print_in_middle(win, 1, 0, x, "Classes" , COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x-2);
	mvwaddch(win, 2, x-1, ACS_RTEE);
	refresh();
	post_menu(menu);
	wrefresh(win);
	data_menu_choice = 0;
	while((c = wgetch(win)))
	{       switch(c)
		{	case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			if(data_menu_choice != n_data_menu - 1)
				data_menu_choice++;
			break;
			case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			if(data_menu_choice != 0)
				data_menu_choice--;
			break;
			case 10: /* Enter */
			remove_menu(n_data_menu);
			return data_menu_choice;
			break;
			default:
			break;
		}
		wrefresh(win);
	}	
	return -1;
}
void add_slot_form() {
	int teacher, subject, class, week_time = 1;	
	char database[128];
	strcpy(database,main_menu_choices[main_menu_choice]);
x:teacher = add_slot_teacher(database);
  if(teacher == n_data_menu - 1)
	  return;
y:subject = add_slot_subject(database);
  if(subject == n_data_menu - 1)
	  goto x;
  class = add_slot_class(database);
  if(class == n_data_menu - 1)
	  goto y;
  create_slot(database,teacher,subject,class,week_time);
}
void show_slot() {
	int n;
	while(1) {
		n = display_slot();
		if(n == n_slot_menu - 1)
			return;
		else if(n == n_slot_menu - 2)
			add_slot_form();	
	}
}
int display_slot() {
	int i, x, y, c, n_teacher, n_subject, n_class, index;
	char slot_menu_choices[128][512], num[16];
	char database[128];
	teacher xteacher[128];
	subject xsubject[128];
	class xclass[32];
	slot xslot[128];	
	strcpy(database,main_menu_choices[main_menu_choice]);
	n_slot_menu = read_slot(database,xslot);
	n_teacher = read_teacher(database,xteacher);
	n_subject = read_subject(database,xsubject);
	n_class = read_class(database,xclass);
	for(i = 0; i < n_slot_menu; i++) {
		sprintf(num,"%d",i+1);
		strcpy(slot_menu_choices[i],num);
		strcat(slot_menu_choices[i]," - ");
		index = get_array_index(xteacher,xslot[i].teacher,n_teacher);
		strcat(slot_menu_choices[i],xteacher[index].name);
		strcat(slot_menu_choices[i]," - ");
		index = get_array_index(xsubject,xslot[i].subject,n_subject);
		strcat(slot_menu_choices[i],xsubject[index].name);
		strcat(slot_menu_choices[i]," - ");
		index = get_array_index(xclass,xslot[i].class,n_class);
		strcat(slot_menu_choices[i],xclass[index].name);
	}	
	strcpy(slot_menu_choices[i], "Add Slot");
	strcpy(slot_menu_choices[i + 1], "Back");
	n_slot_menu+=2;
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	items = (ITEM **)calloc(n_slot_menu + 2, sizeof(ITEM *));
	for(i = 0; i < n_slot_menu; i++) {
		items[i] = new_item(slot_menu_choices[i], NULL);
	}	
	menu = new_menu((ITEM **)items);
	win = newwin(0, 0, 0, 0);
	getmaxyx(win,y,x);
	keypad(win, TRUE);
	set_menu_win(menu, win);
	set_menu_sub(menu, derwin(win, y - 5, 38, 5, 0.4*x));
	set_menu_format(menu,x - 4, 1);
	set_menu_mark(menu, " * ");
	box(win, 0, 0);
	print_in_middle(win, 1, 0, x, "Slots", COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, x - 2);
	mvwaddch(win, 2, x - 1, ACS_RTEE);
	refresh(); 
	post_menu(menu);
	wrefresh(win);
	slot_menu_choice = 0;
	while((c = wgetch(win)))
	{       
		switch(c)
		{	case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			if(slot_menu_choice != n_slot_menu - 1)
				slot_menu_choice++;
			break;
			case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			if(slot_menu_choice != 0)
				slot_menu_choice--;
			break;
			case 10: 
			remove_menu(n_slot_menu);
			return slot_menu_choice;
			default:
			break;
		}
		wrefresh(win);
	}	
	return -1;
}
void generate_timetable() {
	char database[128];
	strcpy(database,main_menu_choices[main_menu_choice]);
	info xinfo;
	population xpopulation;
	read_info(database,&xinfo);
	population_gen(&xpopulation, &xinfo);
	population_evolve(&xpopulation, 50, 10, &xinfo);
	population_sort(&xpopulation, &xinfo);
}
