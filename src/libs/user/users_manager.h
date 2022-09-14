#ifndef USERS_MANAGER_H_
    #define USERS_MANAGER_H_
    int is_user_exist(char *userName, char * passwd_file_path);
    int add_user(char *userName, char *password, int isAdmin, const char *rootPath);
#endif