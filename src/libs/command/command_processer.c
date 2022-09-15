#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "../constants.h"

#include "command_processer.h"

const char *TOK_SPE = " ";

char ** get_args(char *command)
{
    char **tokens = (char**)malloc(MAX_BUFFER_SIZE*sizeof(char*));
    char * cmd = calloc(MAX_BUFFER_SIZE, sizeof(char));
    strcpy(cmd, command);

    int index = 0;
    char *token = strtok(cmd, TOK_SPE);
    while (token != NULL)
    {
        *(tokens+index) = token;
        token = strtok(NULL, TOK_SPE);
        index++;
    }

    *(tokens+index) = NULL;

    return tokens;

    
}