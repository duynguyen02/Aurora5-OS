#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"
#include "../../libs/user/users_manager.h"

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}

int rmrf(char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}



int delete_folder(char *filename, char *rootPath)
{
    UserInfo *user = get_current_user(rootPath);

    if (user == NULL)
    {
        printf("Something went wrong, try again!\n");
        return ERROR_EXIT_STATUS;
    }

    char *final_path = calloc(MAX_BUFFER_SIZE, sizeof(char));

    strcpy(final_path, user->current_dir);
    strcat(final_path, "/");
    strcat(final_path, filename);

    if (is_dir_exists(final_path) == 0)
    {
        printf("rmdir: can not create folder\n");
        printf("Folder does not exists\n");
        return ERROR_EXIT_STATUS;
    }

    rmrf(final_path);

    return SUCESS_EXIT_STATUS;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        printf("rmdir: missing file operand\n");
        printf("Try 'rmdir --help' for more information.\n");
        return ERROR_EXIT_STATUS;
    }

    if (argc > 3)
    {
        printf("rmdir: Error: to many arguments, try again!\n");
        return ERROR_EXIT_STATUS;
    }

    if (strcmp(argv[1], "--help\n") == 0)
    {
        printf("Usage: rmdir [dirname]\n");
        printf("Delete a folder\n");
        return SUCESS_EXIT_STATUS;
    }
    else
    {
        if (argv[1][strlen(argv[1]) - 1] == 10)
        {
            argv[1][strlen(argv[1]) - 1] = '\0';
        }
        return delete_folder(argv[1], argv[argc - 1]);
    }
}