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
# gcc -o out/build/exit exit/exit.c;

# lệnh exit_v2
gcc -o out/build/exit exit/exit_v2.c ./libs/users_manager.a;

# lệnh ls_v2
gcc -o out/build/ls ls/ls_v2.c libs/users_manager.a libs/security.a -lcrypt libs/common.a 

# lệnh ls 
# gcc -o out/build/ls ls/ls.c

# lệnh useradd
gcc -o out/build/useradd useradd/useradd.c libs/users_manager.a libs/security.a -lcrypt libs/common.a

# lệnh su
gcc -o out/build/su su/su.c libs/users_manager.a libs/security.a -lcrypt libs/common.a

# lệnh cd
gcc -o out/build/cd cd/cd.c libs/users_manager.a libs/security.a -lcrypt libs/common.a

# lệnh userdel
gcc -o out/build/userdel userdel/userdel.c libs/users_manager.a libs/security.a -lcrypt libs/common.a

# lệnh touch	
gcc -o out/build/touch touch/touch.c libs/users_manager.a libs/security.a -lcrypt libs/common.a


# lệnh rm
gcc -o out/build/rm rm/rm.c libs/users_manager.a libs/security.a -lcrypt libs/common.a

# lệnh mkdir
gcc -o out/build/mkdir mkdir/mkdir.c libs/users_manager.a libs/security.a -lcrypt libs/common.a

# lệnh rmdir
gcc -o out/build/rmdir rmdir/rmdir.c libs/users_manager.a libs/security.a -lcrypt libs/common.a

# lệnh zhuling
gcc -o out/build/zhuling zhuling/zhuling.c -lcurl libs/common.a

# lệnh shutdown
gcc -o out/build/shutdown shutdown/shutdown.c

# lệnh writef
gcc -o out/build/writef writef/writef.c libs/common.a

# lệnh cat
gcc -o out/build/cat cat/cat.c libs/common.a

### Xoá toàn bộ file object ###
rm *.o;

echo "Build sucessful!";

