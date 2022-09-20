all: aurora5

aurora5: aurora5.c  aucolors.o common.o security.o first_setup.o shell.o users_manager.o command_processer.o process_manager.o
	gcc -lcrypt aurora5.c  aucolors.o common.o security.o -lcrypt first_setup.o shell.o users_manager.o command_processer.o process_manager.o -o aurora5

first_setup.o: libs/os/first_setup.c
	gcc -c libs/os/first_setup.c

shell.o: libs/os/shell.c
	gcc -c libs/os/shell.c

common.o: libs/utils/common.c
	gcc -c libs/utils/common.c

aucolors.o: libs/utils/aucolors.c
	gcc -c libs/utils/aucolors.c

security.o: libs/security/security.c
	gcc libs/security/security.c -c 

users_manager.o: libs/user/users_manager.c
	gcc -c libs/user/users_manager.c

command_processer.o: libs/command/command_processer.c
	gcc -c libs/command/command_processer.c

process_manager.o: libs/process/process_manager.c
	gcc -c libs/process/process_manager.c

clean: 
	rm *.o
	rm aurora5
	rm -rf ./etc ./bin ./home ./root




# [ -e aurora5c ] && rm aurora5c
# gcc aurora5.c libs/setup/shell.c libs/utils/aucolors.c libs/utils/common.c libs/setup/first_setup.c  -o demo;

# gcc -lcrypt aurora5.c  aucolors.o common.o security.o -lcrypt first_setup.o shell.o users_manager.o -o aurora5

