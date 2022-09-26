#include "stdio.h"
#include "stdlib.h"
#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <signal.h>

void sigintHandler(int sig_num)
{
    /* Reset handler to catch SIGINT next time.
    Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, sigintHandler);
    printf("\n Cannot be terminated using Ctrl+C \n");
    fflush(stdout);
}

int main(){
    signal(SIGINT, sigintHandler);
    setlocale(LC_ALL, "ja_JP.UTF8");
    printf("%s\n","君の名は ~ Kimi no Na wa");
    printf("%s","Your name: ");
    char * name = calloc(255, sizeof(char));
    fgets(name, 255,stdin);
    name[strlen(name) - 1] = '\0';
    printf("Konnichiwa %s!\n", name);
    return 0;
}