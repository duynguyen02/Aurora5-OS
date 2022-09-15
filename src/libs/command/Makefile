all: test

test: command_processer.o test.c
	gcc test.c command_processer.o -o test

command_processer.o: command_processer.c
	gcc -c command_processer.c

clean: 
	rm *.o test