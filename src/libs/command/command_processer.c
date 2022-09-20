#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <dirent.h>

#include "../constants.h"
#include "../utils/common.h"
#include "../process/process_manager.h"

#include "command_processer.h"

const char *TOK_SPE = " ";

char *create_bin_file_path(const char *rootPath, const char *command)
{
    // tạo đường dẫn tuyệt đối cho bin
    char *bin_dir_path;
    bin_dir_path = malloc(strlen(rootPath) + strlen(BIN_DIR) + strlen(command) + 1);
    strcpy(bin_dir_path, rootPath);
    strcat(bin_dir_path, BIN_DIR);
    strcat(bin_dir_path, "/");
    strcat(bin_dir_path, command);
    return bin_dir_path;
}

char **get_args(char *command)
{
    char **tokens = (char **)malloc(MAX_BUFFER_SIZE * sizeof(char *));
    char *cmd = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(cmd, command);

    int index = 0;
    char *token = strtok(cmd, TOK_SPE);
    while (token != NULL)
    {
        *(tokens + index) = token;
        token = strtok(NULL, TOK_SPE);
        index++;
    }

    *(tokens + index) = NULL;

    return tokens;
}

int execute_command(char *command, char *rootPath)
{
    char **args = get_args(command);

    // nếu không nhập lệnh gì cả
    if (strlen(*args) == 1)
    {
        return 0;
    }

    // tìm kiếm các câu lệnh trong bin
    // nếu không có thì xuất command not found
    char * bin_path = create_bin_file_path(rootPath, *args);
    char *start = &bin_path[0];
    char *end = &bin_path[strlen(bin_path)];

    char *path = calloc(MAX_BUFFER_SIZE, sizeof(char));
    memcpy(path, start, end - start);

    if (path[strlen(path)-1] == 10){
        path[strlen(path)-1] = '\0';
    }

    if (is_file_exists(path) == 0)
    {
        printf("Command not found: %s\n", *args);
        return 2;
    }

    return run_thread(path, args);
}