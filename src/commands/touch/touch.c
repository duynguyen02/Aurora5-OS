#include "stdio.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

int create_file(char *filename, char *rootPath)
{
    UserInfo *user = get_current_user(rootPath);

    if (user == NULL){
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_STATUS;
    }

    char * final_path = calloc(MAX_BUFFER_SIZE, sizeof(char));

    strcpy(final_path, user->current_dir);
    strcat(final_path, "/");
    strcat(final_path, filename);

    if (is_file_exists(final_path) == 1){
        printf("touch: can not create file\n");
        printf("File exists\n");
        return ERROR_EXIT_STATUS;
    }

    FILE *fp;
    fp  = fopen (final_path, "w");
    fclose(fp);
    return SUCESS_EXIT_STATUS;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        printf("touch: missing file operand\n");
        printf("Try 'touch --help' for more information.\n");
        return ERROR_EXIT_STATUS;
    }

    if (argc > 3)
    {
        printf("touch: Error: to many arguments, try again!\n");
        return ERROR_EXIT_STATUS;
    }

    if (strcmp(argv[1], "--help\n") == 0)
    {
        printf("Usage: touch [filename]\n");
        return SUCESS_EXIT_STATUS;
    }
    else
    {
        if (argv[1][strlen(argv[1]) - 1] == 10)
        {
            argv[1][strlen(argv[1]) - 1] = '\0';
        }
        return create_file(argv[1], argv[argc - 1]);
    }
}