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
#ifndef _TIMETABLE_H
#define _TIMETABLE_H
#include <menu.h>
#include "database.h"
typedef struct timetable{
	int index;
	int teacher;
	int subject;
	int batch;
	int slot;
}timetable;
/*This function adds a timetable to the database */
int add_timetable(char *databse, timetable *xtimetable);
int remove_timetable(char *database, int index);
/*This function removes the timetable from the given database */
int edit_timetable(char *database, int index, timetable *xtimetable);
/*This function edits the name of the timetable from the given database */
timetable get_timetable(char *database, int n);
/* Returns the nth timetable in the database , irrespective of the index*/
int timetable_number(char *database);
/*Returns the number of timetables in the database */
int *find_timetable_info(char *database, int index);
/*This function returns a list of subjects taught by the timetable */
int sort_timetable(char *database , int(*compare )(const void *x ,const void *y));
/*This functon sorts the list of timetables according the compare function */
int start_timetable(char *database);
/*This is the main display function for timetables , it calls all other functions */
int timetable_menu(char *database);
/*This function shows the timetable menu */
int timetable_submenu(char *database,int index);
int timetable_form(char *database);
/*This handles all the forms of the timetable datastructure */
int timetable_teacher(char *database);
int timetable_batch(char *database);
int timetable_subject(char *database);
int get_array_index(char *database, int datum, int index);
int get_index(char *database, int datum, int index);
#endif
