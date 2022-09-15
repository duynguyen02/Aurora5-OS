#include "shell.h"
#include "stdlib.h"
#include "string.h"

#include "../constants.h"

#include "../utils/aucolors.h"

const char *OS_NAME_L1 = "  ___                            _____        _____ _____ ";
const char *OS_NAME_L2 = " / _ \\                          |  ___|      |  _  /  ___|";
const char *OS_NAME_L3 = "/ /_\\ \\_   _ _ __ ___  _ __ __ _|___ \\ ______| | | \\ `--. ";
const char *OS_NAME_L4 = "|  _  | | | | '__/ _ \\__/ _ \\| '__/ _`|\\ \\___| | | |`--. \\";
const char *OS_NAME_L5 = "| | | | |_| | | | (_) | | | (_| /\\__/ /      \\ \\_/ /\\__/ /";
const char *OS_NAME_L6 = "\\_| |_/\\__,_|_|  \\___/|_|  \\__,_\\____/        \\___/\\____/ ";

void welcome_message(const char *version){
    printf("\e[1;1H\e[2J");
    text_color(RED);
    printf("%s\n", OS_NAME_L1);
    text_color(MAG);
    printf("%s\n", OS_NAME_L2);
    text_color(YEL);
    printf("%s\n", OS_NAME_L3);
    text_color(GRN);
    printf("%s\n", OS_NAME_L4);
    text_color(BLU);
    printf("%s\n", OS_NAME_L5);
    text_color(MAG);
    printf("%s\n", OS_NAME_L6);
    text_color(RESET);
    printf("-----WELCOME TO AURORA5-0S v%s-----\n\n", version);
};

void progress_bar(float iteration, float total){

    int length = 30;
    float percent = ((iteration / total) * 100);
    int filled_length = (int)(length * iteration /total);
    // printf("\u2586");

    char * bar = calloc(MAX_BUFFER_SIZE, sizeof(char));

    for (int i=0; i < filled_length; i++){
        strcat(bar, "\u2586");
    }

    for (int i=0; i < (length-filled_length); i++){
        strcat(bar, "-");
    }

    printf("\r%s |%s| %.2f%%\n", "->", bar, percent);


    if (iteration == total){
        printf("\n");
    }
    
}

void clear_c(){
    printf("\e[1;1H\e[2J");
}

const char * ROOT_SYMBOL = "/";

void aurora_shell(const char* userName, const char* hostName, const char * rootDir,char* currentDir, int exitCode){
    char * final_current_dir;
    char * status = (exitCode == 0) ? "\x1B[32m[\u2713]" : "\x1B[31m[\u2613]";

    if (strcmp(rootDir, currentDir) == 0){
        final_current_dir = (char*)calloc(strlen(ROOT_SYMBOL) + 1,sizeof(char));
        strcpy(final_current_dir, ROOT_SYMBOL);
    }
    else{
        char *start = &currentDir[strlen(rootDir)];
        char *end = &currentDir[strlen(currentDir)];

        final_current_dir = (char*)calloc(MAX_BUFFER_SIZE,sizeof(char));

        memcpy(final_current_dir, start, end-start);

    }




    
    printf("%s%s[%s]%s \u2665 %s[%s]%s[%s]\n%s",status,MAG ,userName, RED,BLU,hostName, GRN, final_current_dir ,RESET);
    printf("\u22D9  ");
    text_color(RESET);
}