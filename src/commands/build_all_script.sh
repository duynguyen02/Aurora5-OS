if [ -d "./out" ]
then
	rm -rf out;
fi

if [ -d "./libs" ]
then
	rm -rf libs;
fi


mkdir -p out/build;
mkdir libs;

### xây dựng các thư viện ###

## users_manager	
gcc -c ../libs/user/users_manager.c -o users_manager.o;
ar rcs ./libs/users_manager.a users_manager.o;

## security
gcc -c ../libs/security/security.c -o security.o;
ar rcs ./libs/security.a security.o;

## common
gcc -c ../libs/utils/common.c -o common.o;
ar rcs ./libs/common.a common.o;





# lệnh clear
gcc -o out/build/clear clear/clear.c;

# lệnh echo
gcc -o out/build/echo echo/echo.c;

# lệnh exit
gcc -o out/build/exit exit/exit.c;

# lệnh exit_v2
gcc -o out/build/exit_v2 exit/exit_v2.c ./libs/users_manager.a;

# lệnh ls_v2
gcc -o out/build/ls_v2 ls/ls_v2.c libs/users_manager.a libs/security.a -lcrypt libs/common.a 

# lệnh ls 
gcc -o out/build/ls ls/ls.c

# lệnh useradd
gcc -o out/build/useradd useradd/useradd.c libs/users_manager.a libs/security.a -lcrypt libs/common.a


### Xoá toàn bộ file object ###
rm *.o;

echo "Build sucessful!";

