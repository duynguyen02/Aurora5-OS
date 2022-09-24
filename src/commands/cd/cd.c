#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/user/users_manager.h"

int change_to_user_dir(const char *rootPath)
{
    UserInfo *user = get_current_user(rootPath);

    if (user == NULL)
    {
        printf("Shell do not have any user!\n");
        return EXIT_STATUS;
    }

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

int change_to_root_dir(const char *rootPath)
{
    UserInfo *user = get_current_user(rootPath);

    if (user == NULL)
    {
        printf("Shell do not have any user!\n");
        return EXIT_STATUS;
    }

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

int change_to_parent_dir(const char *rootPath)
{
    UserInfo *user = get_current_user(rootPath);

    if (user == NULL)
    {
        printf("Shell do not have any user!\n");
        return EXIT_STATUS;
    }

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

int change_to_dir(char *rootPath, char *path)
{
    char *final_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
    UserInfo *user = get_current_user(rootPath);
    if (user == NULL)
    {
        printf("Shell do not have any user!\n");
        return EXIT_STATUS;
    }

    // nếu ký tự đầu tiền là '.' cd sẽ thực hiện đến các thư mục con của thư mục hiện tại
    if (path[0] == '.')
    {
        strcpy(final_path, user->current_dir);

        char *temp_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
        strcpy(temp_path, path);

        if (path[1] == '/')
        {
            // nếu sau kí tự / còn ký tự tức là user muốn vào thư mục con
            if (path[2] != '\0')
            {
                char *start = &temp_path[1];
                char *end = &temp_path[strlen(temp_path)];

                char *substr = (char *)calloc(1, end - start + 1);

                memcpy(substr, start, end - start);

                strcat(final_path, substr);
            }

            // nếu thư mục không tồn tại thì báo lỗi
            if (is_dir_exists(final_path) == 0)
            {
                printf("cd: no such file or directory\n");
                return ERROR_EXIT_STATUS;
            }

            strcpy(user->current_dir, final_path);

            if (replace_the_last_shell_user(rootPath, *user) == 0)
            {
                printf("Something went wrong, try again!\n");
                return ERROR_EXIT_STATUS;
            }

            return SUCESS_EXIT_STATUS;
        }
        else
        {
            strcat(final_path, "/");

            char *start = &temp_path[0];
            char *end = &temp_path[strlen(temp_path)];

            char *substr = (char *)calloc(1, end - start + 1);

            memcpy(substr, start, end - start);

            strcat(final_path, substr);

            // nếu thư mục không tồn tại thì báo lỗi
            if (is_dir_exists(final_path) == 0)
            {
                printf("cd: no such file or directory\n");
                return ERROR_EXIT_STATUS;
            }

            strcpy(user->current_dir, final_path);

            if (replace_the_last_shell_user(rootPath, *user) == 0)
            {
                printf("Something went wrong, try again!\n");
                return ERROR_EXIT_STATUS;
            }

            return SUCESS_EXIT_STATUS;
        }
    }
    // nếu ký tự đầu tiền là '~' cd sẽ thực hiện đến các thư mục con của thư mục người dùng
    else if (path[0] == '~')
    {
        strcpy(final_path, get_user_dir(rootPath, user->current_user));

        if (path[1] == '/')
        {
            // nếu sau kí tự / không còn kí tự sẽ chuyển về thư mục người dùng.
            if (path[2] == '\0')
            {
                return change_to_user_dir(rootPath);
            }

            char *temp_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
            strcpy(temp_path, path);

            strcat(final_path, "/");

            char *start = &temp_path[2];
            char *end = &temp_path[strlen(temp_path)];

            char *substr = (char *)calloc(1, end - start + 1);

            memcpy(substr, start, end - start);

            strcat(final_path, substr);

            // nếu thư mục không tồn tại thì báo lỗi
            if (is_dir_exists(final_path) == 0)
            {
                printf("cd: no such file or directory\n");
                return ERROR_EXIT_STATUS;
            }

            strcpy(user->current_dir, final_path);

            if (replace_the_last_shell_user(rootPath, *user) == 0)
            {
                printf("Something went wrong, try again!\n");
                return ERROR_EXIT_STATUS;
            }

            return SUCESS_EXIT_STATUS;
        }
        else
        {
            printf("Error location, try again!\n");
            return ERROR_EXIT_STATUS;
        }
    }

    // nếu ký tự đầu tiền là '/' cd sẽ thực hiện đến các thư mục con của thư mục root
    else if (path[0] == '/')
    {
        strcpy(final_path, rootPath);

        if (path[1] != '/')
        {

            char *temp_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
            strcpy(temp_path, path);

            strcat(final_path, "/");

            char *start = &temp_path[1];
            char *end = &temp_path[strlen(temp_path)];

            char *substr = (char *)calloc(1, end - start + 1);

            memcpy(substr, start, end - start);

            strcat(final_path, substr);

            // nếu thư mục không tồn tại thì báo lỗi
            if (is_dir_exists(final_path) == 0)
            {
                printf("cd: no such file or directory\n");
                return ERROR_EXIT_STATUS;
            }

            strcpy(user->current_dir, final_path);

            if (replace_the_last_shell_user(rootPath, *user) == 0)
            {
                printf("Something went wrong, try again!\n");
                return ERROR_EXIT_STATUS;
            }

            return SUCESS_EXIT_STATUS;
        }
        else
        {
            printf("Error location, try again!\n");
            return ERROR_EXIT_STATUS;
        }
    }
    else
    {
        strcpy(final_path, user->current_dir);
        strcat(final_path, "/");
        strcat(final_path, path);

        // nếu thư mục không tồn tại thì báo lỗi
        if (is_dir_exists(final_path) == 0)
        {
            printf("cd: no such file or directory\n");
            return ERROR_EXIT_STATUS;
        }

        strcpy(user->current_dir, final_path);

        if (replace_the_last_shell_user(rootPath, *user) == 0)
        {
            printf("Something went wrong, try again!\n");
            return ERROR_EXIT_STATUS;
        }

        return SUCESS_EXIT_STATUS;
    }

    printf("Something went wrong, try again!\n");
    return ERROR_EXIT_STATUS;
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
        else if (strcmp(argv[1], ".") == 0)
        {
            return SUCESS_EXIT_STATUS;
        }
        else
        {
            return change_to_dir(argv[argc - 1], argv[1]);
        }
    }

    printf("cd: Error: too many or few arguments, try again!\n");

    return ERROR_EXIT_STATUS;
}