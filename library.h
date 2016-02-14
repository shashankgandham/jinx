/*    
    This file is part of Xtimetable originally written by Shashank Gandham. Xtimetable is free software: you can redistribute it and/or 
    modifyvit under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, 
    or (at your option) any later version.Xtimetable is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
    details. You should have received a copy of the GNU General Public License along with Xtimetable.  If not, see <http://www.gnu.org/licenses/>.
*/

//HEADERS
#include <menu.h>

//DATA-STRUCTURES
typedef struct alloc{
	int index;
	int teacher;
	int subject;
	int batch;
	int slot;
}alloc;
typedef struct batch{
	int index;
	int parent;
	char name[128];
	int strength;
}batch;
typedef struct room{
	int index;
	char name[128];
	int capacity;
}room;
typedef struct subject{
	int index;
	char name[128];
	int slot_len;
	int slot_per_week;
}subject;
typedef struct teacher{
	int index;
	char name[128];
	int week_time;
}teacher;
typedef struct timetable{
	int index;
	int teacher;
	int subject;
	int batch;
	int slot;
}timetable;

//GUI && I/O
void remove_menu(MENU *menu, ITEM **items, int n);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
int scanstr(WINDOW *win, char *str, int len);
int scanint(WINDOW *win);
void print_form_str(WINDOW *win, int y, int x);
void print_form_int(WINDOW *win, int y, int x);
//DATABASE
int new_database(char *name);
void new_database_form();
int show_database_menu(char *database);
int database_menu(char *database);
void get_database(int choice, char *choices);
int database_number();
int main_menu();
int main();

//ALLOC
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

//BATCH
int add_batch(char *database, batch *xbatch);
/*This function adds a batch to the database */
int remove_batch(char *database, int index);
/*This function removes the batch from the given database */
int edit_batch(char *database, int index, batch *xbatch);
/*This function edits the name of the batch from the given database */
batch get_batch(char *database, int n);
/* Returns the nth batch in the database , irrespective of the index*/
int batch_number(char *database);
/*Returns the number of batchs in the database */
int sort_batch(char *database , int(*compare )(const void *x ,const void *y));
/*This functon sorts the list of batchs according the compare function */
int start_batch(char *database);
/*This is the main display function for batchs , it calls all other functions */
int batch_menu(char *database);
/*This function shows the batch menu */
int alloc_form_batch(char *database, int index);
int batch_submenu(char *database,int index);
int batch_form(char *database);
/*This handles all the forms of the batch datastructure */

//ROOM
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
int sort_room(char *database , int(*compare )(const void *x ,const void *y));
/*This functon sorts the list of rooms according the compare function */
int start_room(char *database);
/*This is the main display function for rooms , it calls all other functions */
int room_menu(char *database);
/*This function shows the room menu */
int room_submenu(char *database,int index);
int room_form(char *database);
/*This handles all the forms of the room datastructure */

//SUBJECT
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
int sort_subject(char *database , int(*compare )(const void *x ,const void *y));
/*This functon sorts the list of subjects according the compare function */
int start_subject(char *database);
/*This is the main display function for subjects , it calls all other functions */
int subject_menu(char *database);
/*This function shows the subject menu */
int alloc_form_subject(char *database, int index);
int subject_submenu(char *database,int index);
int subject_form(char *database);
/*This handles all the forms of the subject datastructure */

//TEACHER
int add_teacher(char *database, teacher *xteacher);
/*This function adds a teacher to the database */
int remove_teacher(char *database, int index);
/*This function removes the teacher from the given database */
int edit_teacher(char *database, int index, teacher *xteacher);
/*This function edits the name of the teacher from the given database */
teacher get_teacher(char *database, int n);
/* Returns the nth teacher in the database , irrespective of the index*/
int teacher_number(char *database);
/*Returns the number of teachers in the database */
int sort_teacher(char *database , int(*compare )(const void *x ,const void *y));
/*This functon sorts the list of teachers according the compare function */
int start_teacher(char *database);
/*This is the main display function for teachers , it calls all other functions */
int teacher_menu(char *database);
/*This function shows the teacher menu */
int teacher_submenu(char *database,int index);
int alloc_form_teacher(char *database, int index);
int teacher_form(char *database);
/*This handles all the forms of the teacher datastructure */

//TIMETABLE
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
