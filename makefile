# Makefile for Timetable Generator by Shashank Gandham liscensed under GPLv2
personal : delete
	@echo "The Program was compiled successfully and its details are :"
	@echo "Lines Bytes Filename"
	@wc -lc *.c *.h
delete: ttmaker
	@rm *.o

ttmaker: display.o database.o generator.o
	@gcc -o project display.o database.o generator.o -lncurses -lgsl -lgslcblas -lmenu -g

database.o: database.c library.h
	@gcc -c -g -Wall database.c -o database.o 

display.o: display.c library.h
	@gcc -c -g -Wall display.c -o display.o 

generator.o: generator.c library.h
	@gcc -c -g -Wall generator.c -o generator.o 
clean :
	@sudo rm *.o ttmaker *.dat
