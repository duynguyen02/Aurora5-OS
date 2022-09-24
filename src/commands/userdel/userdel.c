#include "stdio.h"
#include "string.h"

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

int delete_user(char * username, char * rootPath){
    UserInfo *user = get_current_user(rootPath);

    if (strcmp(username, ADMIN_USER_NAME) == 0){
        printf("You can not delete root user!\n");
        return ERROR_EXIT_STATUS;
    }

    if (is_user_exist(username, rootPath) == 0){
        printf("User: %s does not exist!\n", username);
        return ERROR_EXIT_STATUS;
    }

    if (is_admin(user->current_user, rootPath) == 0){
        printf("You are not a root user to use this command!\n");
        return ERROR_EXIT_STATUS;
    }

    if (is_user_active(username, rootPath) == 1){
        printf("You can not detele '%s', This user is logging on the shell.!\n", username);
        return ERROR_EXIT_STATUS;
    }

    if (remove_user(username, rootPath) == 0){
        return ERROR_EXIT_STATUS;
    }

    printf("Successful!\n");
    return SUCESS_EXIT_STATUS;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        printf("Try 'userdel --help' for more infomation.\n");
        return ERROR_EXIT_STATUS;
    }

    if (argc == 3)
    {
        if (strcmp(argv[1], "--help\n") == 0)
        {
            printf("Usage: userdel [username]\n");
            printf("Delete a user\n");
            return SUCESS_EXIT_STATUS;
        }
        else
        {
            if (argv[1][strlen(argv[1]) - 1] == 10){
                argv[1][strlen(argv[1]) - 1] = '\0';
            }
            return delete_user(argv[1], argv[argc-1]);
        }
    }


    printf("su: Error: to many arguments, try again!\n");

    return ERROR_EXIT_STATUS;
}