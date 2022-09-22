#include "stdio.h"
#include "string.h"
#include<unistd.h>
#include <stdlib.h>

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

int try_to_login(char * username, char * rootPath){
    username[strlen(username)-1] = '\0';

    char * CURRENT_USER = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(CURRENT_USER, username);

    char * ROOT_PATH = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(ROOT_PATH, rootPath);

    if (is_user_exist(username, rootPath) == 0){
        printf("User does not exist.\n");
        return ERROR_EXIT_STATUS;
    }

    char *password = (char *)calloc(MAX_BUFFER_SIZE, sizeof(char));

    printf("Enter your password: ");
    password = strdup(getpass(""));

    if (is_correct_password(username, password, rootPath) == 0){
        printf("su: Authentication failure\n");
        return ERROR_EXIT_STATUS;
    }

    UserInfo *user = malloc(sizeof(UserInfo));

    strcpy(user->current_user, CURRENT_USER);
    strcpy(user->host_name, get_host_name(ROOT_PATH));
    strcpy(user->root_dir, ROOT_PATH);
    strcpy(user->current_dir, get_user_dir(ROOT_PATH, CURRENT_USER));
    user->is_admin = (is_admin(CURRENT_USER, ROOT_PATH) == 1) ? 1 : 0;


    if (append_user_to_shell(rootPath, *user) == 0){
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_STATUS;
    }

    return SUCESS_EXIT_STATUS;

}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        printf("Try 'su --help' for more infomation.\n");
        return ERROR_EXIT_STATUS;
    }

    if (argc == 3)
    {
        if (strcmp(argv[1], "--help\n") == 0)
        {
            printf("Usage: su [username]\n");
            printf("Login to another users\n");
            return SUCESS_EXIT_STATUS;
        }
        else
        {
            return try_to_login(argv[1], argv[argc-1]);
        }
    }


    printf("su: Error: to many arguments, try again!\n");

    return ERROR_EXIT_STATUS;
}