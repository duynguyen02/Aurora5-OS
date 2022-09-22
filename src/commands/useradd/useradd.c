#include "string.h"

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

int try_to_add_user(char *username, char *password, char *is_admin, char *rootPath)
{
    if (strchr(username, ':') != NULL)
    {
        printf("':' symbol is not allowed\n");
        return 2;
    }

    if (is_user_exist(username, rootPath) == 1)
    {
        printf("User: %s exists!\n", username);
        return 2;
    }

    int is_invalid_key = 1;

    if ((is_admin[0] != 121))
    {
        is_invalid_key = 0;
    }

    if (is_invalid_key == 0)
    {
        if ((is_admin[0] != 110))
        {
            printf("Invalid key '%c'", is_admin[0]);
            return ERROR_EXIT_STATUS;
        }
    }

    if (add_user(
            username,
            password,
            (is_admin[0] == 121) ? 1 : 0,
            rootPath) == 1)
    {
        printf("Successful!\n");
        return 0;
    }
    else
    {
        printf("Something went wrong, try again!\n");
        return 2;
    }
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        printf("Try 'useradd --help' for more infomation.\n");
        return 2;
    }

    if (argc == 3)
    {
        if (strcmp(argv[1], "--help\n") == 0)
        {
            printf("Usage: useradd [username] [password] [is admin?(y/n)]\n");
            return 0;
        }
        else
        {
            printf("useradd: invalid option -- '%s'\n", argv[1]);
            return 2;
        }
    }

    if (argc == 5)
    {
        UserInfo *user = get_current_user(argv[4]);

        if (user->is_admin != 1)
        {
            printf("You are not root user to use this command!\n");
            return 2;
        }

        return try_to_add_user(argv[1], argv[2], argv[3], argv[4]);
    }

    printf("useradd: Error: to many arguments, try again!\n");

    return 2;
}