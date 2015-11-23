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
#ifndef _SUBJECT_H
#define _SUBJECT_H
typedef struct subject{
	int index;
	char name[128];
	int slot_len;
	int slot_per_week;
}subject;
int add_subject(char *database, subject *xsubject);
/*This function adds a subject to the database */

int remove_subject(char *database, int index);
/*This function removes the subject from the given database */

int edit_subject(char *database, int index, subject *xsubject);
/*This function edits the name of the subject from the given database */
subject get_subject(char *database, int n);
/* Returns the nth subject in the database , irrespective of the index*/
int subject_number(char *database);
/*Returns the number of subjects in the database */

int *find_subject_info(char *database, int index);
/*This function returns a list of subjects taught by the subject */ 

int sort_subject(char *database , int(*compare )(const void *x ,const void *y));
/*This functon sorts the list of subjects according the compare function */

int start_subject(char *database);
/*This is the main display function for subjects , it calls all other functions */

int subject_menu(char *database);
/*This function shows the subject menu */

int subject_submenu(char *database,int index);
int subject_form(char *database);
/*This handles all the forms of the subject datastructure */
#endif
