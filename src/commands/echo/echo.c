/**
 * @author Nguyễn Phùng Hưng
 * Câu lệnh in chữ ra màn hình Shell
 */
#include "stdio.h"

#include "../../libs/constants.h"

int main(int argc, char **argv){
    if (argc <= 2){
        printf("\n");
    }
    else{
        // đọc các đối số và in ra màn hinh
        for (int i = 1; i < argc-1; i++){
            printf("%s ",argv[i]);
        }
        
    }
    printf("\n");

    return SUCCESS_EXIT_CODE;
}