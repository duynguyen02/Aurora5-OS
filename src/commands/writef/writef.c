#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libs/constants.h"
#include "../../libs/utils/common.h"

int write_file(char * file_name, char * mode, char * endline, char * root_path);

int main(int argc, char **argv)
{
    if (argc == 5){
        for (int i=0 ;i< argc; i++){
            if (argv[i][strlen(argv[i]) - 1] == 10){
                argv[i][strlen(argv[i]) - 1] = '\0';
            }
        }


        char * mode = calloc(2, sizeof(char));
        if (argv[2][0] == 'w'){
            strcpy(mode, "w");
        }else if (argv[2][0] == 'a')
        {
            strcpy(mode, "a");
        }
        else{
            printf("Unknown mode: %s\n",argv[2]);
            return ERROR_EXIT_CODE;
        }

        char * file_path = calloc(MAX_BUFFER_SIZE, sizeof(char));
        strcpy(file_path, argv[argc-1]);
        strcat(file_path,"/");
        strcat(file_path,argv[1]);

        return write_file(file_path, mode, argv[3], argv[argc - 1]);

        
    }
    printf("Usage: writef [file name] [mode(a: append/ w:write)][endline]\n");
    return ERROR_EXIT_CODE;
}

int write_file(char * file_name, char * mode, char * endline, char * root_path){

    char * str = calloc(MAX_BUFFER_SIZE, sizeof(char));
    char * str_temp = calloc(MAX_BUFFER_SIZE, sizeof(char));
    while (1)
    {
        fgets(str_temp, MAX_BUFFER_SIZE, stdin);
        if (strcmp(str_temp, endline) != 10){
            strcat(str, str_temp);
        }
        else{
            break;
        }
    }
    
    FILE *file;
    file = fopen(file_name, mode);
    fputs(str, file);

    fclose(file);

    return SUCCESS_EXIT_CODE;
    

}