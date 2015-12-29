# Makefile for Timetable Generator by Shashank Gandham liscensed under GPLv2
personal : jinx
	@echo "The Program was compiled successfully and its details are :"
	@echo "Lines Bytes Filename"
	@wc -lc *.c *.h

jinx: .teacher.o .batch.o .subject.o .room.o .alloc.o .database.o .timetable.o
	@gcc -o jinx .batch.o .teacher.o .subject.o .room.o .alloc.o .database.o .timetable.o -lncurses -lgsl -lgslcblas -lmenu -g

.teacher.o: teacher.c
	@gcc -c -Wall teacher.c -o .teacher.o -g

.batch.o: batch.c
	@gcc -c -Wall batch.c -o .batch.o -g

.timetable.o: timetable.c
	@gcc -c -Wall timetable.c -o .timetable.o -g

.subject.o: subject.c
	@gcc -c -Wall subject.c -o .subject.o -g

.room.o: room.c
	@gcc -c -Wall room.c -o .room.o -g

.alloc.o: alloc.c
	@gcc -c -Wall alloc.c -o .alloc.o -g

.database.o: database.c
	@gcc -c -Wall database.c -o .database.o -g
clean:
	rm .*.o
