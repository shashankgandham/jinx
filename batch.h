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
#ifndef alloc_header
	#include "alloc.h"
#endif
typedef struct batch{
	int index;
	int parent;
	char name[128];
	int strength;
}batch;

int add_batch(char *database, batch *xbatch);
/*This function adds a batch to the database */

int remove_batch(char *database, int index);
/*This function removes the batch from the given database */

int edit_batch(char *database, int index, char *name);
/*This function edits the name of the batch from the given database */
batch get_batch(char *database, int n);
/* Returns the nth batch in the database , irrespective of the index*/
int batch_number(char *database);
/*Returns the number of batchs in the database */

alloc *find_batch_info(char *database, int index);
/*This function returns a list of subjects taught by the batch */ 

int sort_batch(char *database , int(*compare )(const void *x ,const void *y));
/*This functon sorts the list of batchs according the compare function */

int start_batch(char *database);
/*This is the main display function for batchs , it calls all other functions */

int batch_menu(char *database);
/*This function shows the batch menu */

int batch_form(char *database);
/*This handles all the forms of the batch datastructure */
