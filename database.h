/*  This file is part of Jinx originally written by Shashank gandham.

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
#include <menu.h>

void remove_menu(MENU *menu, ITEM **items, int n);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
int new_database(char *name);
void new_database_form();
int show_database_menu(char *database);
void database_menu(char *database);
void get_database(int choice, char *choices);
int database_number();
int main_menu();
int main();
