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
#define _TEACHER_H
#ifndef _ALLOC_H
	#include "alloc.h"
#endif

typedef struct teacher{
	int index;
	char name[128];
	int week_time;
}teacher;

/*This function adds a teacher to the database */

int remove_teacher(char *database, int index);
/*This function removes the teacher from the given database */

int edit_teacher(char *database, int index, teacher *xteacher);
/*This function edits the name of the teacher from the given database */
teacher get_teacher(char *database, int n);
/* Returns the nth teacher in the database , irrespective of the index*/
int teacher_number(char *database);
/*Returns the number of teachers in the database */

int *find_teacher_info(char *database, int index);
/*This function returns a list of subjects taught by the teacher */ 

int sort_teacher(char *database , int(*compare )(const void *x ,const void *y));
/*This functon sorts the list of teachers according the compare function */

int start_teacher(char *database);
/*This is the main display function for teachers , it calls all other functions */

int teacher_menu(char *database);
/*This function shows the teacher menu */

int teacher_form(char *database);
/*This handles all the forms of the teacher datastructure */
