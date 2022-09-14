#ifndef USERS_MANAGER_H_
    #define USERS_MANAGER_H_
    int is_user_exist(char *userName,const char *rootPath);
    int add_user(char *userName, char *password, int isAdmin, const char *rootPath);
    int is_correct_password(char * userName, char * password,const char *rootPath);
#endif