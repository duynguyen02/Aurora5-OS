#include "stdio.h"
#include "stdlib.h"

int main(){
    char * name = calloc(255, sizeof(char));
    printf("Your name: \n");
    scanf("%s",name);
    printf("Your name: %s\n", name);
    return 0;
}