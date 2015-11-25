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
#ifndef _ALLOC_H
#define _ALLOC_H
#include <menu.h>
#include "database.h"
typedef struct alloc{
	int index;
	int teacher;
	int subject;
	int batch;
	int slot;
}alloc;
/*This function adds a alloc to the database */
int add_alloc(char *databse, alloc *xalloc);

int remove_alloc(char *database, int index);
/*This function removes the alloc from the given database */

int edit_alloc(char *database, int index, alloc *xalloc);
/*This function edits the name of the alloc from the given database */
alloc get_alloc(char *database, int n);
/* Returns the nth alloc in the database , irrespective of the index*/
int alloc_number(char *database);
/*Returns the number of allocs in the database */

int *find_alloc_info(char *database, int index);
/*This function returns a list of subjects taught by the alloc */

int sort_alloc(char *database , int(*compare )(const void *x ,const void *y));
/*This functon sorts the list of allocs according the compare function */

int start_alloc(char *database);
/*This is the main display function for allocs , it calls all other functions */

int alloc_menu(char *database);
/*This function shows the alloc menu */

int alloc_submenu(char *database,int index);

int alloc_form(char *database);
/*This handles all the forms of the alloc datastructure */
int alloc_teacher(char *database);
int alloc_batch(char *database);
int alloc_subject(char *database);
int get_array_index(char *database, int datum, int index);
int get_index(char *database, int datum, int index);
#endif
