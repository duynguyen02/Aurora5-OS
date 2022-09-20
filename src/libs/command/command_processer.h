#ifndef COMMAND_PROCESSER_H_
    #define COMMAND_PROCESSER_H_
    char ** get_args(char * command);
    int execute_command(char *command, char* rootPath);
#endif