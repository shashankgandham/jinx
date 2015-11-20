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

int new_database(char *name) {
	FILE *fp;
	char database[128][128];
	fp = fopen("database.txt","r");
	int i = 1, d;
	strcpy(database[0],name);
	while((d = fscanf(fp," %[^\n]s",database[i])) && d != EOF ) {
		i++;
	}
	d = i + 1;
	if(i == 128)
       		return -1;
	else {
		fclose(fp);
		remove("database.txt");
		fp = fopen("database.txt","w");
		for(i = 0; i < d; i++)
			fprintf(fp,"%s\n",database[i]);
		
	}
	fclose(fp);
	return 1;
}
void remove_teacher_file(char * database) {
	char file[128];
	strcpy(file,database);
	strcat(file,"_teacher.dat");
	remove(file);
}
void remove_class_file(char * database) {
	char file[128];
	strcpy(file,database);
	strcat(file,"_class.dat");
	remove(file);
}
void remove_room_file(char * database) {
	char file[128];
	strcpy(file,database);
	strcat(file,"_room.dat");
	remove(file);
}
void remove_subject_file(char * database) {
	char file[128];
	strcpy(file,database);
	strcat(file,"_subject.dat");
	remove(file);
}
void remove_slot_file(char * database) {
	char file[128];
	strcpy(file,database);
	strcat(file,"_slot.dat");
	remove(file);
}
void remove_files(char *database) {
	remove_teacher_file(database);
	remove_class_file(database);
	remove_slot_file(database);
	remove_subject_file(database);
	remove_room_file(database);
}
void remove_database(int index) {
	char database[128][128];
	FILE *fp;
	fp = fopen("database.txt","r");
	int i = 0, d;
	while((d = fscanf(fp," %[^\n]s",database[i])) && d != EOF ) {
		i++;
	}
	d = i;
	fclose(fp);
	remove("database.txt");
	fp = fopen("database.txt","w");
	for(i = 0; i < d; i++) {
		if(i != index)
			fprintf(fp,"%s\n",database[i]);
	}
	fclose(fp);
}

int get_index(data *xdata, int index, int n) {
	return xdata[index].index;	
}
int get_array_index(data *xdata, int index, int n) {
	int i;
	for(i = 0; i < n; i++) {
		if(xdata[i].index == index)
			return i;
	}
	return -1;
}
int read_data(char *file_name, data *xdata) {
	FILE *fp;
	int n = -1, d, i;
	char num[16];
	fp = fopen(file_name,"r");
	if(fp == NULL) {
		n = 0;
	}
	else  {
		fscanf(fp,"%[^\n]s",num);
		n = atoi(num);
		i = 0;
		while((d = fscanf(fp," %[^\n]s",xdata[i].name)) && d != EOF ) {
			fscanf(fp," %[^\n]s",num);
			xdata[i].index = atoi(num);
			i++;
		}
		fclose(fp);
	}
	return n;
}
int write_data(char *file_name, char *name) {
	FILE *fp;
	data xdata[128];
	int n,i;
	char num[16];
	n = read_data(file_name, xdata);
	strcpy(xdata[n].name, name);	
	if(n)
		xdata[n].index = xdata[n-1].index + 1;
	else
		xdata[n].index = 0;
	remove(file_name);
	n++;
	sprintf(num,"%d",n);
	fp =  fopen(file_name,"w");	
	fprintf(fp,"%s\n",num);
	i = 0;
	while(i < n) {
		fprintf(fp," %s\n",xdata[i].name);
		sprintf(num,"%d",xdata[i].index);
		fprintf(fp," %s\n",num);
		i++;
	}
	fclose(fp);
	return 1;
}
void remove_data(char *file_name, int index) {
	data xdata[128];
	char num[16];
	int n = read_data(file_name, xdata);
	FILE *fp;
	remove(file_name);
	n--;
	xdata[index] = xdata[n];
	fp = fopen(file_name,"w");
	sprintf(num,"%d",n);
	fprintf(fp,"%s\n",num);
	int i = 0;
	while(i < n) {
		fprintf(fp," %s\n",xdata[i].name);
		sprintf(num,"%d",xdata[i].index);
		fprintf(fp," %s\n",num);
		i++;
	}
	fclose(fp);
}
void edit_data(char *file_name, char *name, int index) {
	data xdata[128];
	char num[16];
	int n = read_data(file_name, xdata);
	FILE *fp;
	remove(file_name);
	strcpy(xdata[index].name, name);
	fp = fopen(file_name,"w");
	sprintf(num,"%d",n);
	fprintf(fp,"%s\n",num);
	int i = 0;
	while(i < n) {
		fprintf(fp," %s\n",xdata[i].name);
		sprintf(num,"%d",xdata[i].index);
		fprintf(fp," %s\n",num);
		i++;
	}
	fclose(fp);
}
int read_class(char *database, class *xclass) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_class.dat");
	return read_data(file_name, xclass);
}
int read_room(char *database, room *xroom) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_room.dat");
	return read_data(file_name, xroom);
}
int read_teacher(char *database, teacher *xteacher) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_teacher.dat");
	return read_data(file_name, xteacher);
}
int read_subject(char *database, subject *xsubject) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_subject.dat");
	return read_data(file_name, xsubject);
}
void add_class(char *database, char *name) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_class.dat");
	write_data(file_name,name);
}
void add_teacher(char *database, char *name) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_teacher.dat");
	write_data(file_name,name);
}
void add_subject(char *database, char *name) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_subject.dat");
	write_data(file_name,name);
}
void add_room(char *database, char *name) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_room.dat");
	write_data(file_name,name);
}
void edit_class(char *database, char *name, int index) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_class.dat");
	edit_data(file_name,name,index);
}
void edit_teacher(char *database, char *name, int index) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_teacher.dat");
	edit_data(file_name,name,index);
}
void edit_subject(char *database, char *name, int index) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_subject.dat");
	edit_data(file_name,name, index);
}
void edit_room(char *database, char *name, int index) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_room.dat");
	edit_data(file_name, name, index);
}
void remove_room(char *database, int index) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_room.dat");
	remove_data(file_name, index);
}
void remove_class(char *database, int index) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_class.dat");
	remove_data(file_name, index);
}
void remove_subject(char *database, int index) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_room.dat");
	remove_data(file_name, index);
}
void remove_teacher(char *database, int index) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_teacher.dat");
	remove_data(file_name, index);
}

int read_slot(char *database, slot *xslot) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_slot.dat");
	int i = 0, d;
	FILE *fp;
	fp = fopen(file_name, "r");
	if(fp == NULL) {
		return 0;
	}
	else {
		while((d = fscanf(fp,"%d%d%d%d",&xslot[i].teacher,&xslot[i].subject,&xslot[i].class,&xslot[i].week_time)) && d != EOF)
			i++;
	}
	fclose(fp);
	return i;
}
void write_slot(char *database, slot *xslot, int n) {
	char file_name[128];
	strcpy(file_name,database);
	strcat(file_name,"_slot.dat");
	int i;
	FILE *fp;
	fp = fopen(file_name, "a");
	i = 0;
	while(i < n) {
		fprintf(fp,"%d %d %d %d\n",xslot[i].teacher,xslot[i].subject,xslot[i].class,xslot[i].week_time);
		i++;
	}
	fclose(fp);
}
void create_slot(char *database, int teacher, int subject, int class, int week_time) {
	int n, i;
	slot xslot[128];
	n = 0;
	for(i = 0; i < week_time; i++) {
		xslot[n].teacher = teacher;
		xslot[n].subject = subject;
		xslot[n].class = class;
		xslot[n].week_time = week_time;
		n++;
	}
	write_slot(database,xslot,n);
}
void read_info(char *database, info *xinfo) {
	data xdata[128];
	xinfo->teacher_number = read_teacher(database, xdata);
	xinfo->subject_number = read_subject(database, xdata);
	xinfo->class_number = read_class(database, xdata);
	xinfo->room_number = read_room(database, xdata);
	xinfo->day_slot = 7;
	xinfo->xslot = (slot *)malloc(sizeof(slot)*128);
	xinfo->slot_number = read_slot(database, xinfo->xslot);
	xinfo->swaps = 5;
	xinfo->penalty_teacher = -100;
	xinfo->penalty_class = -100;
	xinfo->population_number = 500;
}
