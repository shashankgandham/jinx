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

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <gsl/gsl_math.h>
#include <ncurses.h>
#define TEACHER 1
#define ROOM 2
#define CLASS 3
#define SUBJECT 4
#define MAIN 10
#define DATABASE 20
#define DATA 30
#define EDIT_DATA 40
#define SLOT 50
typedef struct timetable{
	int *time;
	double fitness;
	int *slot;
	int len;
}timetable;
typedef struct slot{
	int index;
	int subject;
	int teacher;
	int class;
	int week_time;
}slot;
typedef struct data {
	int index;
	char name[128];
	int week_time;
}data;
typedef data teacher;
typedef data room;
typedef data subject;
typedef data class;

typedef struct info{
	int subject_number;
	int teacher_number;
	int class_number;
	int slot_number;
	int room_number;
	int time_number;
	int day_slot;
	slot *xslot;
	int swaps;
	double penalty_teacher;
	double penalty_class;
	int population_number;
}info;
typedef struct population {
	timetable *xtimetable;
	int generation;
	int population_number;
}population;


/*------------------------------display.c------------------------------*/
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
int get_menu_choices();
void quit();
void new_database_form();
int show_database_menu();
void show_teacher();
void show_subject();
void show_class();
void show_room();
void show_slot();
int display_slot();
void diplay_data(char *filename);
void generate_timetable();
void database_main_menu();
void main_database_menu();
void remove_menu(int marker);
int display_data(char *database, int marker);
void new_database_form();
/*------------------------------database.c-----------------------------*/
int new_database(char *name);

int get_index(data *xdata, int index, int n);
int get_array_index(data *xdata, int index, int n);
int read_data(char *file_name, data *xdata);
int write_data(char *file_name, char *name);
void remove_data(char *file_name, int index);
int read_class(char *database, class *xclass);
int read_room(char *database, class *xroom);
int read_teacher(char *database, class *xteacher);
int read_subject(char *database, class *xsubject);
void add_class(char *database, char *name);
void add_teacher(char *database, char *name);
void add_subject(char *database, char *name);
void add_room(char *database, char *name);
void remove_room(char *database, int index);
void remove_class(char *database, int index);
void remove_subject(char *database, int index);
void remove_teacher(char *database, int index);
int read_slot(char *database, slot *xslot);
void write_slot(char *database, slot *xslot, int n);
void create_slot(char *database, int teacher, int subject, int class, int week_time);
void read_info(char *database, info *xinfo);
void edit_teacher(char *database,char *name,int n);
void edit_subject(char *database,char *name,int n);
void edit_class(char *database,char *name,int n);
void edit_room(char *database,char *name,int n);
void remove_database(int index);
void remove_files(char *database);
/*------------------------------generator.c----------------------------*/
void timetable_init(timetable *xtimetable, info *xinfo);
void timetable_gen(timetable *xtimetable, info *xinfo);
int timetable_check_empty(timetable *xtimetable, int slot, info *xinfo);
double fitness(timetable *xtimetable, info *xinfo);
void population_gen(population *xpopulation, info *xinfo);
void population_sort(population *xpopulation, info *xinfo);
void timetable_crossover(timetable *parent_a,timetable *parent_b,timetable *child_a,timetable *child_b, info *xinfo);
void timetable_mutate(timetable *child, info *xinfo);
void population_evolve(population *xpopulation, int generations, double fitness,info *xinfo);
