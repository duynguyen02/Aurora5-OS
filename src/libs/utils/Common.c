#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "Constants.h"

/**
 *
 */
char *get_current_dir()
{
    char *cwd = calloc(MAX_CWD_SIZE, sizeof(char));
    cwd = getcwd(NULL, 0);
    return cwd;
}

/**
 *
 */
int is_file_exists(const char *fileName)
{
    FILE *file;

    if (file = fopen(fileName, "r"))
    {
        fclose(file);
        return 1;
    }

    return 0;
}

/**
 *
 */
int is_dir_exists(const char *dirName)
{

    struct stat stats;

    stat(dirName, &stats);

    if (S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}

void delay(int number_of_seconds){
    int milli_seconds = 1000 * number_of_seconds;

    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds)
        ;
}
