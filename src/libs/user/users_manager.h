#ifndef USERS_MANAGER_H_
#define USERS_MANAGER_H_
#define MAX_OF_USER 100
typedef struct 
{
    char current_user[MAX_BUFFER_SIZE];
    char host_name[MAX_BUFFER_SIZE];
    char root_dir[MAX_BUFFER_SIZE];
    char current_dir[MAX_BUFFER_SIZE];
    int is_admin;
} UserInfo;

int is_user_exist(char *userName, const char *rootPath);
int add_user(char *userName, char *password, int isAdmin, const char *rootPath);
int is_correct_password(char *userName, char *password, const char *rootPath);
char *get_host_name(const char *rootPath);
char *get_user_dir(const char *rootPath, const char *username);
int add_user_to_shell(const char * rootPath ,UserInfo user);
UserInfo* get_current_user(const char * rootPath);
int is_admin(char *userName,const char *rootPath);
#endif