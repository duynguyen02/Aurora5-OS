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

int is_user_exist(char *userName, char *rootPath)
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

int add_user(char *userName, char *password, int isAdmin, const char *rootPath)
{
    // tạo đường dẫn tuyệt đối cho file passwd
    char *passwd_file_path = create_passwd_file_path(rootPath);

    // nếu đường dẫn không tồn tại
    if (is_file_exists(passwd_file_path) == 0)
    {
        return 0;
    }

    // if (is_user_exist(userName, passwd_file_path) == 1)
    // {
    //     printf("okok");
    //     return 0;
    // }

    // tiến hành mở file và thêm user
    FILE *file_ptr = fopen(passwd_file_path, "a");
    if (file_ptr != NULL)
    {
        printf("mo file");
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