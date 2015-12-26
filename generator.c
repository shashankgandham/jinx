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
#include "library.h"
#include <assert.h>
#include <time.h>
long random_int(unsigned min, unsigned max) {
	int r;
	srand(time(NULL));
	while((r = rand()%max)) {
		if(r > min)
			return r;
	}
	return r;
}
void timetable_integrity (timetable *xtimetable , info *xinfo) {
	int i;
	for(i = 0; i < xinfo->slot_number; i++) {
		assert(xtimetable->time[xtimetable->slot[i]] == i);
	}
}
void timetable_init(timetable *xtimetable,  info *xinfo) {
	xtimetable->time = (int *)malloc(sizeof(int) * xinfo->day_slot * xinfo->room_number);	
	xtimetable->fitness = 0;
	xtimetable->slot = (int *)malloc(sizeof(slot) * xinfo->slot_number);	
	int i;
	for(i = 0; i < (xinfo->day_slot * xinfo->room_number); i++)
		xtimetable->time[i] = -1;
}
void timetable_gen(timetable *xtimetable, info *xinfo) {
	int i, slot, empty;
	for(i = 0; i < xinfo->slot_number; i++) {
		slot = random_int(0, (xinfo->room_number * xinfo->day_slot));
		empty = timetable_check_empty(xtimetable, slot, xinfo);
		xtimetable->time[empty] = i;
		xtimetable->slot[i] = empty;
	}
	timetable_integrity(xtimetable, xinfo);
	xtimetable->fitness = fitness(xtimetable, xinfo);
}
int compare(const void *a, const void *b) {
	if((*(timetable *)a).fitness > (*(timetable *)b).fitness)
		return 1;
	else if((*(timetable *)a).fitness < (*(timetable *)b).fitness)
		return -1;
	else
		return 0;
}
void population_sort(population *xpopulation, info *xinfo) {
	qsort(xpopulation->xtimetable, xinfo->population_number,sizeof(timetable),compare);
}
void population_gen(population *xpopulation, info *xinfo) {
	xpopulation->generation = 0;
	xpopulation->xtimetable = (timetable *)malloc(sizeof(timetable) * xinfo->population_number);
	
	int i;
	for(i = 0; i < (xinfo->population_number); i++) {
		timetable_init(&(xpopulation->xtimetable[i]), xinfo);
		timetable_gen(&(xpopulation->xtimetable[i]), xinfo);
	}
}
void population_evolve(population *xpopulation, int generations, double fitness,info *xinfo) {
	double i;
	population_sort(xpopulation, xinfo);
	int child_a, child_b, parent_a, parent_b;
	timetable *a_timetable = NULL, *b_timetable =NULL;
	for(i = FLT_MIN; (i < fitness) && (xpopulation->generation < generations); xpopulation->generation++) {
		child_a = xinfo->population_number - 1;
		child_b = xinfo->population_number - 2;
		parent_a = 0;
		parent_b = 1;
		timetable_crossover(&(xpopulation->xtimetable[parent_a]),&(xpopulation->xtimetable[parent_b]),a_timetable,b_timetable,xinfo);
		xpopulation->xtimetable[child_a] = *a_timetable;	
		xpopulation->xtimetable[child_b] = *b_timetable;
		timetable_mutate(&(xpopulation->xtimetable[child_a]), xinfo);
		timetable_mutate(&(xpopulation->xtimetable[child_b]), xinfo);
		population_sort(xpopulation, xinfo);	
		i = xpopulation->xtimetable[0].fitness;
	}
}

double fitness(timetable *xtimetable, info *xinfo) {
	timetable_integrity(xtimetable, xinfo);
	int slot = 0, room = 0;
	double fitness = 0;
	int slot_a, slot_b;
	int clash_teacher = -1,  clash_class = -1;
	int time;
	for(slot = 0; slot < xinfo->slot_number; slot++) {
		clash_teacher = -1;
		clash_class = -1;
		slot_a = xtimetable->slot[slot];
		time = slot/(xinfo->room_number);
		for (room = 0; room < xinfo->room_number; room++) {
			slot_b = xtimetable->time[time * xinfo->room_number + room];
			if (slot_b == -1) 
				continue;
			if (xinfo->xslot[slot_b].teacher == xinfo->xslot[slot_a].teacher)
				clash_teacher += 1;

			if (xinfo->xslot[slot_a].class == xinfo->xslot[slot_b].class) {
				clash_class += 1;
			}
		}
		fitness += (clash_teacher * xinfo->penalty_teacher);
		fitness += (clash_class* xinfo->penalty_class);
	}
	return fitness;
}

int timetable_check_empty(timetable *xtimetable, int cur, info *xinfo) {
		int r, l, rlimit;
		l = r = cur;
		rlimit = xinfo->slot_number * xinfo->room_number;
		assert(l >= 0);
		//assert(r < rlimit);
		while (((xtimetable->time[r] != -1) && (xtimetable->time[l] != -1)) && !((l == 0) && (r == rlimit))) {
			if (r + 1 < rlimit) 
				r++;
			if (l - 1 >= 0)
				l--;
			assert(l >= 0);
			//assert(r < rlimit);
		}
		if (xtimetable->time[r] == -1) {
			return r;
		}
		if (xtimetable->time[l]  == -1) {
			return l;
		}
		return -1;
}

void timetable_crossover(timetable *parent_a,timetable *parent_b, timetable *child_a, timetable *child_b, info *xinfo) {
	timetable_integrity(parent_a, xinfo);
	timetable_integrity(parent_b, xinfo);
	printw("check integrity\n");
	refresh();
	system("sleep 1");
	int crossover_1, crossover_2;
	crossover_1 = random_int(0, xinfo->slot_number);
	crossover_2 = random_int(crossover_1, xinfo->slot_number + 1);
	timetable_init(child_a, xinfo);
	timetable_init(child_b, xinfo);	
	printw("init\n");
	refresh();
	system("sleep 1");
	child_a = parent_a;
	child_b = parent_b;
	timetable_integrity(child_a, xinfo);
	timetable_integrity(child_b, xinfo);
	printw("init\n");
	refresh();
	system("sleep 1");
	int i, temp;
	for(i = crossover_1; i < crossover_2; i++) {
		(child_a)->time[(child_a)->slot[i]] = -1;
		(child_b)->time[(child_b)->slot[i]] = -1;
	}

	for(i = crossover_1; i < crossover_2; i++) {
		temp = (child_a)->slot[i];
		(child_a)->slot[i] = (child_b)->slot[i];
		(child_b)->slot[i] = temp;
		(child_a)->slot[i] = timetable_check_empty(child_a,(child_a)->slot[i],xinfo);
		(child_b)->slot[i] = timetable_check_empty(child_b,(child_b)->slot[i],xinfo);
		(child_a)->time[(child_a)->slot[i]] = i;
		(child_b)->time[(child_b)->slot[i]] = i;
	}
	timetable_integrity(child_a, xinfo);
	timetable_integrity(child_b, xinfo);
	(child_a)->fitness = fitness(child_a, xinfo);
	(child_b)->fitness = fitness(child_b, xinfo);
}

void timetable_mutate(timetable *xtimetable,info *xinfo) {
	timetable_integrity(xtimetable, xinfo);
	int swaps, i, j, temp;
	for(swaps = 0; swaps < xinfo->swaps; swaps++) {
		i = random_int(0, xinfo->slot_number);
		j = random_int(0, xinfo->slot_number);
		temp = xtimetable->slot[i];
		xtimetable->slot[i] = xtimetable->slot[j];
		xtimetable->slot[j] = temp;

		xtimetable->time[xtimetable->slot[i]] = i;
		xtimetable->time[xtimetable->slot[j]] = j;
	}
	timetable_integrity(xtimetable, xinfo);
	xtimetable->fitness = fitness(xtimetable, xinfo);
}

