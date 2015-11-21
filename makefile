# Makefile for Timetable Generator by Shashank Gandham liscensed under GPLv2

personal : remove	
	@echo "The Program was compiled successfully and its details are :"
	@echo "Lines Bytes Filename"
	@wc -lc *.c *.h
remove: jinx
	rm *.o
jinx: teacher.o batch.o subject.o room.o alloc.o database.o
	@gcc -o jinx batch.o teacher.o subject.o room.o alloc.o database.o -lncurses -lgsl -lgslcblas -lmenu -g

teacher.o: teacher.c
	@gcc -c -Wall teacher.c -g

batch.o: batch.c
	@gcc -c -Wall batch.c -g

subject.o: subject.c
	@gcc -c -Wall subject.c -g

room.o: room.c
	@gcc -c -Wall room.c -g

alloc.o: alloc.c
	@gcc -c -Wall alloc.c -g

database.o: database.c
	@gcc -c -Wall database.c -g
