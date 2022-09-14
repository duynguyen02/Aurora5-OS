test: test.c users_manager.o security.o common.o
	mkdir 
	gcc test.c common.o users_manager.o security.o -lcrypt -o test

users_manager.o: users_manager.c
	gcc -c users_manager.c

security.o: ../security/security.c
	gcc ../security/security.c -c 

common.o: ../utils/common.c
	gcc -c ../utils/common.c

clean:
	rm *.o test