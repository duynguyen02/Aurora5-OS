#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

#include "../utils/aucolors.h"
#include "../utils/Constants.h"
#include "../utils/Common.h"
#include "shell.h"



void first_time_setup(const char * osName, const char * osVersion){
    text_color(GRN);
    printf("Setting Up Your %s For The First Time\n", osName);
    text_color(RESET);

    char *computer_name = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    char *root_password = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    char c;

    printf("Your computer name: ");
    scanf("%[^\n]s",computer_name);


    printf("Enter your root password : ");

    root_password = getpass("");

    for (int i = 0; i <= 100; i+=5){
        welcome_message(osVersion);
        printf("PLEASE WAIT...\n");
        progress_bar(i, 100);
        delay(200);
    }



}