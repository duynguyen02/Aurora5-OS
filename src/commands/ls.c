#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define ETC_DIR "/etc"
#define USER_SHELL_FILE "/usershell.bin"

#define MAX_OF_USER 100
#define MAX_BUFFER_SIZE 100

#define AUCOLORS_H_
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

typedef struct
{
    char current_user[MAX_BUFFER_SIZE];
    char host_name[MAX_BUFFER_SIZE];
    char root_dir[MAX_BUFFER_SIZE];
    char current_dir[MAX_BUFFER_SIZE];
} UserInfo;

UserInfo *get_current_user(const char *rootPath)
{
    char *user_shell_path = calloc(strlen(rootPath) + strlen(ETC_DIR) + strlen(USER_SHELL_FILE) + 1, sizeof(char));

    strcpy(user_shell_path, rootPath);
    strcat(user_shell_path, ETC_DIR);
    strcat(user_shell_path, USER_SHELL_FILE);

    FILE *file;
    file = fopen(user_shell_path, "r");

    if (file == NULL)
    {
        return NULL;
    }

    unsigned int n_stud = 0;

    UserInfo users[MAX_OF_USER];

    while (fread(&users[n_stud], sizeof(UserInfo), 1, file) == 1)
    {
        n_stud++;
    }

    fclose(file);

    if (n_stud == 0)
    {
        return NULL;
    }

    UserInfo *l_user = &users[n_stud-1];

    return l_user;
}

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