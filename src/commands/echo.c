#include "stdio.h"

int main(int argc, char **argv){
    if (argc <= 2){
        printf("\n");
    }
    else{
        for (int i = 1; i < argc-1; i++){
            printf("%s ",argv[i]);
        }
        
    }
    printf("\n");

    return 0;
}