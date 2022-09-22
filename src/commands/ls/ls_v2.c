#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"
#include "../../libs/utils/aucolors.h"



#include "../../libs/user/users_manager.h"

int list_current_dir(char *rootPath)
{
    UserInfo *user = get_current_user(rootPath);
    if (user != NULL)
    {
        DIR *direntory;
        struct dirent *entry;
        direntory = opendir(user->current_dir);

        if (direntory == NULL)
        {
            // char *start = &user->current_dir[strlen(rootPath)];
            // char *end = &user->current_dir[strlen(user->current_dir)];

            // char * final_current_dir = (char*)calloc(255,sizeof(char));

            // memcpy(final_current_dir, start, end-start);

            printf("ls: cannot access '.': Error opening directory.\n");
            return 2;
        }

        while ((entry = readdir(direntory)) != NULL)
        {
            if (entry->d_type == DT_REG)
            {
                printf("%s\n", entry->d_name);
            }
            else if (entry->d_type == DT_DIR)
            {
                printf("%s%s%s\n", BLU,entry->d_name, RESET);
            }
        }

        printf("\n");

        closedir(direntory);

        return 0;
    }
    return 2;
}


int main(int argc, char **argv)
{
    if (argc == 2)
    {
        return list_current_dir(argv[1]);
    }

    printf("Something went wrong, try again!");

    return 2;
}