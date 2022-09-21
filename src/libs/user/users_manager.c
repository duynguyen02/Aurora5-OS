#include "string.h"
#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"

#include "../utils/common.h"
#include "../security/security.h"

#include "users_manager.h"



char *create_passwd_file_path(const char *rootPath)
{
    // tạo đường dẫn tuyệt đối cho file passwd
    char *passwd_file_path;
    passwd_file_path = malloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(PASSWD_FILE) + 1);
    strcpy(passwd_file_path, rootPath);
    strcat(passwd_file_path, ETC_DIR);
    strcat(passwd_file_path, PASSWD_FILE);
    return passwd_file_path;
}

char *create_hostname_file_path(const char *rootPath)
{
    // tạo đường dẫn tuyệt đối cho file passwd
    char *passwd_file_path;
    passwd_file_path = malloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(HOST_NAME_FILE) + 1);
    strcpy(passwd_file_path, rootPath);
    strcat(passwd_file_path, ETC_DIR);
    strcat(passwd_file_path, HOST_NAME_FILE);
    return passwd_file_path;
}

int is_user_exist(char *userName,const char *rootPath)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(create_passwd_file_path(rootPath), "r");
    if (fp == NULL)
        return 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *token = strtok(line, ":");
        if (strcmp(userName, token) == 0)
        {
            return 1;
        }
    }

    fclose(fp);
    if (line)
        free(line);
    return 0;
}

int is_correct_password(char *userName, char *password,const char *rootPath)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(create_passwd_file_path(rootPath), "r");
    if (fp == NULL)
        return 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *token = strtok(line, ":");
        if (strcmp(userName, token) == 0)
        {
            token = strtok(NULL, ":");
            if(strcmp(encrypt_sha512(password), token) == 0){
                return 1;
            }
        }
    }

    fclose(fp);
    if (line)
        free(line);
    return 0;
}

int add_user(char *userName, char *password, int isAdmin, const char *rootPath)
{
    // tạo đường dẫn tuyệt đối cho file passwd
    char *passwd_file_path = create_passwd_file_path(rootPath);

    // nếu đường dẫn không tồn tại
    if (is_file_exists(passwd_file_path) == 0)
    {
        return 0;
    }

    if (is_user_exist(userName, rootPath) == 1)
    {
        return 0;
    }

    // tiến hành mở file và thêm user
    FILE *file_ptr = fopen(passwd_file_path, "a");
    if (file_ptr != NULL)
    {
        // mã hóa mật khẩu
        char *encrypt_pass = encrypt_sha512(password);
        char *isAdminStr = (isAdmin == 1) ? "1" : "0";

        /// tạo chuỗi được format để thêm vào file ///
        char *user_info = malloc(strlen(userName) + strlen(encrypt_pass) + 3);
        strcpy(user_info, userName);
        strcat(user_info, ":");
        strcat(user_info, encrypt_pass);
        strcat(user_info, ":");
        strcat(user_info, isAdminStr);
        strcat(user_info, "\n");
        fputs(user_info, file_ptr);
        fclose(file_ptr);
        return 1;
    }

    return 0;
}

char * get_host_name(const char *rootPath){
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(create_hostname_file_path(rootPath), "r");
    if (fp == NULL)
        return "";

    while ((read = getline(&line, &len, fp)) != -1)
    {
        return line;
    }

    fclose(fp);
    if (line)
        free(line);
    return "";
}

char * get_user_dir(const char *rootPath , const char *username){
    char * user_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
    
    if (strcmp(username, ADMIN_USER_NAME) == 0){
        strcpy(user_path, rootPath);
        return user_path;
    }

    char * home_path = calloc(MAX_BUFFER_SIZE, sizeof(char));

    strcpy(home_path, HOME_DIR);
    strcat(home_path, "/");
    strcat(home_path, username);


    strcpy(user_path, rootPath);
    strcat(user_path, home_path);   
    
    return user_path;
}

int add_user_to_shell(const char * rootPath ,UserInfo user){
    char * user_shell_path = calloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(USER_SHELL_FILE) + 1, sizeof(char));

    strcpy(user_shell_path, rootPath);
    strcat(user_shell_path, ETC_DIR);
    strcat(user_shell_path, USER_SHELL_FILE);

    UserInfo users[1];

    users[0] = user;

    FILE *file;
    file = fopen(user_shell_path, "w");

    if(file == NULL){
        return 0;
    }


    fwrite(users,sizeof(UserInfo), 1, file);

    fclose(file);

    return 1;
}


UserInfo* get_current_user(const char * rootPath){
    char * user_shell_path = calloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(USER_SHELL_FILE) + 1, sizeof(char));

    strcpy(user_shell_path, rootPath);
    strcat(user_shell_path, ETC_DIR);
    strcat(user_shell_path, USER_SHELL_FILE);

    FILE *file;
    file = fopen(user_shell_path, "r");

    if(file == NULL){
        return NULL;
    }

    unsigned int n_stud = 0;

    UserInfo users[MAX_OF_USER];

    while (fread(&users[n_stud], sizeof(UserInfo), 1, file) == 1)
    {
        n_stud++;
    }


    fclose(file);

    if (n_stud == 0){
        return NULL;
    }

    UserInfo *l_user = &users[n_stud-1];

    return l_user;
}
