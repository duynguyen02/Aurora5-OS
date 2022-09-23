#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "../../libs/constants.h"
#include "../../libs/user/users_manager.h"

int change_to_user_dir(char *rootPath)
{
    UserInfo *user = get_current_user(rootPath);

    if (strcmp(user->current_user, ADMIN_USER_NAME) == 0)
    {
        strcpy(user->current_dir, rootPath);
    }
    else
    {
        strcpy(user->current_dir, get_user_dir(rootPath, user->current_user));
    }
    if (replace_the_last_shell_user(rootPath, *user) == 0)
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_STATUS;
    }
    return SUCESS_EXIT_STATUS;
}

int change_to_root_dir(char *rootPath)
{
    UserInfo *user = get_current_user(rootPath);

    strcpy(user->current_dir, rootPath);

    if (replace_the_last_shell_user(rootPath, *user) == 0)
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_STATUS;
    }
    return SUCESS_EXIT_STATUS;
}

char *get_parent_dir(const char *path)
{
    int index = -1;
    for (int i = strlen(path) - 1; i >= 0; i--)
    {
        if (path[i] == '/')
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        return "";
    }

    char *temp_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(temp_path, path);

    char *start = &temp_path[0];
    char *end = &temp_path[index];

    char *substr = (char *)calloc(1, end - start + 1);
    memcpy(substr, start, end - start);

    return substr;
}

int change_to_parent_dir(char *rootPath)
{
    UserInfo *user = get_current_user(rootPath);

    if (strcmp(user->current_dir, rootPath) != 0)
    {
        if (strcmp(get_parent_dir(user->current_dir), "") == 0)
        {
            printf("Something went wrong, try again!\n");
            return ERROR_EXIT_STATUS;
        }
        strcpy(user->current_dir, get_parent_dir(user->current_dir));
    }

    if (replace_the_last_shell_user(rootPath, *user) == 0)
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_STATUS;
    }

    return SUCESS_EXIT_STATUS;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        return change_to_user_dir(argv[argc - 1]);
    }

    if (argc == 3)
    {
        if (argv[1][strlen(argv[1]) - 1] == 10)
        {
            argv[1][strlen(argv[1]) - 1] = '\0';
        }

        if (strcmp(argv[1], "~") == 0)
        {
            return change_to_user_dir(argv[argc - 1]);
        }
        else if (strcmp(argv[1], "/") == 0)
        {
            return change_to_root_dir(argv[argc - 1]);
        }
        else if (strcmp(argv[1], "..") == 0)
        {
            return change_to_parent_dir(argv[argc - 1]);
        }
    }

    // if (argc == 5)
    // {
    //     UserInfo *user = get_current_user(argv[4]);

    //     if (user->is_admin != 1)
    //     {
    //         printf("You are not root user to use this command!\n");
    //         return 2;
    //     }

    //     return try_to_add_user(argv[1], argv[2], argv[3], argv[4]);
    // }

    printf("useradd: Error: to many arguments, try again!\n");

    return ERROR_EXIT_STATUS;
}