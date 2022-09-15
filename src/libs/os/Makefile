test: test.c shell.o aucolors.o
	gcc test.c shell.o aucolors.o -o test

shell.o: shell.c
	gcc shell.c -c

aucolors.o: ../utils/aucolors.c
	gcc ../utils/aucolors.c -c

clean:
	rm *.o test