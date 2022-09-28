#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        for (int i = 0; i < argc; i++)
        {
            if (argv[i][strlen(argv[i]) - 1] == 10)
            {
                argv[i][strlen(argv[i]) - 1] = '\0';
            }
        }

        char *file_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
        strcpy(file_path, argv[argc - 1]);
        strcat(file_path, "/");
        strcat(file_path, argv[1]);

        if (is_file_exists(file_path) == 0)
        {
            printf("File do not exist!\n");
            return ERROR_EXIT_CODE;
        }

        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        fp = fopen(file_path, "r");
        if (fp == NULL)
            return 0;

        while ((read = getline(&line, &len, fp)) != -1)
        {
            printf("%s", line);
        }

        fclose(fp);
        if (line)
            free(line);
        return 0;
    }

    printf("Usage: cat [file name]\n");
    return ERROR_EXIT_CODE;
}