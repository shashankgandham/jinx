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
#ifndef _ROOM_H
#define _ROOM_H
typedef struct room{
	int index;
	char name[128];
	int capacity;
}room;

int add_room(char *database, room *xroom);
/*This function adds a room to the database */

int remove_room(char *database, int index);
/*This function removes the room from the given database */

int edit_room(char *database, int index, room *xroom);
/*This function edits the name of the room from the given database */
room get_room(char *database, int n);
/* Returns the nth room in the database , irrespective of the index*/
int room_number(char *database);
/*Returns the number of rooms in the database */

int *find_room_info(char *database, int index);
/*This function returns a list of subjects taught by the room */ 

int sort_room(char *database , int(*compare )(const void *x ,const void *y));
/*This functon sorts the list of rooms according the compare function */

int start_room(char *database);
/*This is the main display function for rooms , it calls all other functions */

int room_menu(char *database);
/*This function shows the room menu */

int room_form(char *database);
/*This handles all the forms of the room datastructure */
#endif
