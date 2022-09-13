#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../utils/aucolors.h"
#include "../utils/Constants.h"
#include "../utils/Common.h"
#include "shell.h"

const char * HOST_NAME_FILE = "/hostname";

void first_time_setup(const char * osName, const char * osVersion, const char * etcAbsolutePath){
    char * host_name_file_path = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));

    strcpy(host_name_file_path, etcAbsolutePath);
    strcat(host_name_file_path,HOST_NAME_FILE);



    text_color(GRN);
    printf("Setting Up Your %s For The First Time\n", osName);
    text_color(RESET);

    char *computer_name = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    
    printf("Your computer name: ");
    scanf("%[^\n]s",computer_name);


    char *root_password;
    char *comfirm_root_password;


    int correct_pass = 0;
    while (!correct_pass)
    {
        root_password = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
        comfirm_root_password = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));

        printf("Enter your root password : ");
        root_password = strdup(getpass(""));
        
        printf("Confirm your root password : ");
        comfirm_root_password = strdup(getpass(""));

        if (strcmp(root_password, comfirm_root_password) == 0){
            correct_pass = 1;
        }else{
            printf("Incorrect comfirm password, please try again!\n");
            free(root_password);
            free(comfirm_root_password);
        }
    }

    struct stat st = {0};


    // cần tạo thư mục etc trước
    mkdir(etcAbsolutePath, 0777);

    // sau đó mới tạo file hostname
    FILE* file_ptr = fopen(host_name_file_path, "w");
    fputs(computer_name, file_ptr);
    fclose(file_ptr);

    
    for (int i = 0; i <= 100; i+=5){
        welcome_message(osVersion);
        printf("PLEASE WAIT...\n");
        progress_bar(i, 100);
        delay(200);
    
    }

    

}

